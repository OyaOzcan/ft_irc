#include "common.hpp"
#include "handle.hpp"

Server::Server() : server_fd(-1) {}

Server::~Server() {
    if (server_fd != -1) close(server_fd);
}

const std::map<std::string, Channel>& Server::getChannels() const {
    return channels;
}

std::map<std::string, Channel>& Server::getChannels() {
    return channels;
}

void Server::setup(int port) {
    struct sockaddr_in address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket oluşturulamadı");
        exit(1);
    }

    fcntl(server_fd, F_SETFL, O_NONBLOCK);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind hatası");
        close(server_fd);
        exit(1);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen hatası");
        close(server_fd);
        exit(1);
    }

    pollfd server_pollfd = {server_fd, POLLIN, 0};
    poll_fds.push_back(server_pollfd);
}

void Server::run() {
    while (true) {
        int poll_count = poll(poll_fds.data(), poll_fds.size(), -1);
        if (poll_count < 0) {
            perror("Poll hatası");
            break;
        }

        for (size_t i = 0; i < poll_fds.size(); ++i) {
            if (poll_fds[i].fd == server_fd && (poll_fds[i].revents & POLLIN)) {
                int client_fd = accept(server_fd, NULL, NULL);
                if (client_fd < 0) {
                    perror("Accept hatası");
                    continue;
                }

                fcntl(client_fd, F_SETFL, O_NONBLOCK);

                pollfd client_pollfd = {client_fd, POLLIN, 0};
                poll_fds.push_back(client_pollfd);

                std::cout << "Yeni bir istemci bağlandı: " << client_fd << std::endl;
            } else if (poll_fds[i].revents & POLLIN) {
                char buffer[1024] = {0};
                int bytes_read = recv(poll_fds[i].fd, buffer, sizeof(buffer), 0);
                if (bytes_read > 0) {
                    std::cout << "İstemci " << poll_fds[i].fd << " mesaj gönderdi: " << buffer << std::endl;
                    handleClientMessages(poll_fds[i].fd, buffer, *this);
                } else {
                    close(poll_fds[i].fd);
                    poll_fds.erase(poll_fds.begin() + i);
                    --i;
                }
            }
        }
    }
}

void Server::handleJoinCommand(int client_fd, const std::string& channel_name) {
    if (channels.find(channel_name) == channels.end()) {
        channels[channel_name] = Channel(channel_name);
        std::cout << "Yeni kanal oluşturuldu: " << channel_name << std::endl;
    }

    channels[channel_name].addMember(client_fd);
    std::cout << "İstemci " << client_fd << " kanala katıldı: " << channel_name << std::endl;

    const std::vector<int>& members = channels[channel_name].getMembers();
    for (size_t i = 0; i < members.size(); ++i) {
        int member_fd = members[i];
        if (member_fd != client_fd) {
            std::ostringstream oss;
            oss << "Kullanıcı " << client_fd << " kanala katıldı: " << channel_name << "\n";
            std::string join_msg = oss.str();
            send(member_fd, join_msg.c_str(), join_msg.size(), 0);
        }
    }
}

void Server::removeClient(int client_fd) {
	for (size_t i = 0; i < poll_fds.size(); ++i) {
		if (poll_fds[i].fd == client_fd) {
			close(client_fd);
			poll_fds.erase(poll_fds.begin() + i);
			break;
		}
	}

	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
		it->second.removeMember(client_fd);
	}

	std::cout << "İstemci " << client_fd << " bağlantısı sonlandırıldı." << std::endl;
}

void Server::handleKickCommand(int client_fd, const std::string& target) {
    std::istringstream iss(target);
    std::string channel_name, kicked_user_fd_str;
    iss >> channel_name >> kicked_user_fd_str;

    if (channels.find(channel_name) == channels.end()) {
        std::string error_msg = "Hata: Kanal bulunamadı -> " + channel_name + "\n";
        send(client_fd, error_msg.c_str(), error_msg.size(), 0);
        return;
    }

    int kicked_user_fd = atoi(kicked_user_fd_str.c_str());
    Channel& channel = channels[channel_name];

    if (!channel.isOperator(client_fd)) {
        std::string error_msg = "Hata: Operatör değilsiniz.\n";
        send(client_fd, error_msg.c_str(), error_msg.size(), 0);
        return;
    }

    channel.removeMember(kicked_user_fd);
    std::string kick_msg = "Kullanıcı " + kicked_user_fd_str + " kanaldan atıldı.\n";
    channel.broadcastMessage(client_fd, kick_msg);
}

void Server::handleNamesCommand(int client_fd, const std::string& channel_name) {
    if (channels.find(channel_name) == channels.end()) {
        std::string error_msg = "Hata: Kanal bulunamadı -> " + channel_name + "\n";
        send(client_fd, error_msg.c_str(), error_msg.size(), 0);
        return;
    }

    Channel& channel = channels[channel_name];
    const std::vector<int>& members = channel.getMembers();

    std::ostringstream oss;
    oss << "Kanal üyeleri (" << channel_name << "): ";
    for (std::vector<int>::const_iterator it = members.begin(); it != members.end(); ++it) {
        int member_fd = *it;
        oss << member_fd << " ";
    }
    oss << "\n";
    std::string names_msg = oss.str();
    send(client_fd, names_msg.c_str(), names_msg.size(), 0);
}

void Server::handlePartCommand(int client_fd, const std::string& channel_name) {
    if (channels.find(channel_name) == channels.end()) {
        std::string error_msg = "Hata: Kanal bulunamadı -> " + channel_name + "\n";
        send(client_fd, error_msg.c_str(), error_msg.size(), 0);
        return;
    }

    Channel& channel = channels[channel_name];
    channel.removeMember(client_fd);
    std::ostringstream oss;
    oss << "Kullanıcı " << client_fd << " kanaldan ayrıldı.\n";
    std::string part_msg = oss.str();
    channel.broadcastMessage(client_fd, part_msg);
}

void Server::handleModeCommand(int client_fd, const std::string& channel_name, const std::string& mode) {
    if (channels.find(channel_name) == channels.end()) {
        std::string error_msg = "Hata: Kanal bulunamadı -> " + channel_name + "\n";
        send(client_fd, error_msg.c_str(), error_msg.size(), 0);
        return;
    }

    Channel& channel = channels[channel_name];
    if (!channel.isOperator(client_fd)) {
        std::string error_msg = "Hata: Operatör değilsiniz.\n";
        send(client_fd, error_msg.c_str(), error_msg.size(), 0);
        return;
    }

    if (mode == "+i") {
        channel.setInviteOnly(true);
    } else if (mode == "-i") {
        channel.setInviteOnly(false);
    } else if (mode.rfind("+k ", 0) == 0) { // Kanal parolası
        channel.setPassword(mode.substr(3));
    } else if (mode == "-k") {
        channel.setPassword("");
    } else {
        std::string error_msg = "Hata: Geçersiz mod -> " + mode + "\n";
        send(client_fd, error_msg.c_str(), error_msg.size(), 0);
    }
}

void Server::handleInviteCommand(int client_fd, const std::string& target) {
    std::istringstream iss(target);
    std::string channel_name, invited_user_fd_str;
    iss >> channel_name >> invited_user_fd_str;

    int invited_user_fd = atoi(invited_user_fd_str.c_str());
    if (channels.find(channel_name) == channels.end()) {
        std::string error_msg = "Hata: Kanal bulunamadı -> " + channel_name + "\n";
        send(client_fd, error_msg.c_str(), error_msg.size(), 0);
        return;
    }

    Channel& channel = channels[channel_name];
    if (!channel.isOperator(client_fd)) {
        std::string error_msg = "Hata: Operatör değilsiniz.\n";
        send(client_fd, error_msg.c_str(), error_msg.size(), 0);
        return;
    }

    channel.addInvite(invited_user_fd);
    std::string invite_msg = "Kullanıcı " + invited_user_fd_str + " davet edildi.\n";
    send(client_fd, invite_msg.c_str(), invite_msg.size(), 0);
}

void Server::handleTopicCommand(int client_fd, const std::string& channel_name, const std::string& topic) {
    if (channels.find(channel_name) == channels.end()) {
        std::string error_msg = "Hata: Kanal bulunamadı -> " + channel_name + "\n";
        send(client_fd, error_msg.c_str(), error_msg.size(), 0);
        return;
    }

    Channel& channel = channels[channel_name];
    if (!channel.isOperator(client_fd) && !topic.empty()) {
        std::string error_msg = "Hata: Konuyu değiştirme yetkiniz yok.\n";
        send(client_fd, error_msg.c_str(), error_msg.size(), 0);
        return;
    }

    if (!topic.empty()) {
        channel.setTopic(topic);
        std::string topic_msg = "Kanal konusu değiştirildi: " + topic + "\n";
        channel.broadcastMessage(client_fd, topic_msg);
    } else {
        std::string current_topic = channel.getTopic();
        std::string topic_msg = "Mevcut konu: " + current_topic + "\n";
        send(client_fd, topic_msg.c_str(), topic_msg.size(), 0);
    }
}

void Server::handleListCommand(int client_fd) {
    std::ostringstream oss;
    oss << "Kanal listesi:\n";
    for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
        oss << "- " << it->first << "\n";
    }
    std::string list_msg = oss.str();
    send(client_fd, list_msg.c_str(), list_msg.size(), 0);
}


void Server::handleWhoCommand(int client_fd) {
    std::ostringstream oss;
    oss << "Bağlı kullanıcılar:\n";
    for (std::vector<pollfd>::iterator it = poll_fds.begin(); it != poll_fds.end(); ++it) {
        oss << "- Kullanıcı: " << it->fd << "\n";
    }
    std::string who_msg = oss.str();
    send(client_fd, who_msg.c_str(), who_msg.size(), 0);
}


void Server::handleWhoisCommand(int client_fd, const std::string& target) {
    int target_fd = atoi(target.c_str());
    std::ostringstream oss;
    oss << "Kullanıcı bilgisi: " << target_fd << "\n";
    std::string whois_msg = oss.str();
    send(client_fd, whois_msg.c_str(), whois_msg.size(), 0);
}
