#include "common.hpp"

void handleClientMessages(int client_fd, const std::string& message, Server& server) {
    std::istringstream iss(message);
    std::string command, target, content;

    iss >> command >> target;
    std::getline(iss, content);

    // Mesajın başındaki boşlukları temizle
    if (!content.empty() && content[0] == ' ') {
        content.erase(0, 1);
    }

    if (command == "PRIVMSG") {
        if (target.empty() || content.empty()) {
            std::string error_msg = "Hata: Mesaj hedefi veya içeriği eksik.\n";
            send(client_fd, error_msg.c_str(), error_msg.size(), 0);
            return;
        }

        if (server.getChannels().find(target) != server.getChannels().end()) {
            server.getChannels()[target].broadcastMessage(client_fd, content);
            }
            else
            {
                std::string error_msg = "Hata: Kanal bulunamadı -> " + target + "\n";
                send(client_fd, error_msg.c_str(), error_msg.size(), 0);
            }
    }
		else if (command == "JOIN") {
		if (target.empty()) {
			std::string error_msg = "Hata: Kanal adı belirtilmedi.\n";
			send(client_fd, error_msg.c_str(), error_msg.size(), 0);
			return;
		}
		server.handleJoinCommand(client_fd, target);
	} 
	else if (command == "KICK") {
		if (target.empty()) {
			std::string error_msg = "Hata: Kullanıcı adı belirtilmedi.\n";
			send(client_fd, error_msg.c_str(), error_msg.size(), 0);
			return;
		}
		server.handleKickCommand(client_fd, target);
	} 
	else if (command == "WHOIS") {
		if (target.empty()) {
			std::string error_msg = "Hata: Kullanıcı adı belirtilmedi.\n";
			send(client_fd, error_msg.c_str(), error_msg.size(), 0);
			return;
		}
		server.handleWhoisCommand(client_fd, target);
	} 
	else if (command == "WHO") {
		server.handleWhoCommand(client_fd);
	} 
	else if (command == "LIST") {
		server.handleListCommand(client_fd);
	} 
	else if (command == "TOPIC") {
		if (target.empty()) {
			std::string error_msg = "Hata: Kanal adı belirtilmedi.\n";
			send(client_fd, error_msg.c_str(), error_msg.size(), 0);
			return;
		}
		server.handleTopicCommand(client_fd, target, content);
	} 
	else if (command == "INVITE") {
		if (target.empty()) {
			std::string error_msg = "Hata: Kullanıcı adı belirtilmedi.\n";
			send(client_fd, error_msg.c_str(), error_msg.size(), 0);
			return;
		}
		server.handleInviteCommand(client_fd, target);
	} 
	else if (command == "MODE") {
		if (target.empty()) {
			std::string error_msg = "Hata: Kanal adı belirtilmedi.\n";
			send(client_fd, error_msg.c_str(), error_msg.size(), 0);
			return;
		}
		server.handleModeCommand(client_fd, target, content);
	} 
	else if (command == "PART") {
		if (target.empty()) {
			std::string error_msg = "Hata: Kanal adı belirtilmedi.\n";
			send(client_fd, error_msg.c_str(), error_msg.size(), 0);
			return;
		}
		server.handlePartCommand(client_fd, target);
	} 
	else if (command == "NAMES") {
		if (target.empty()) {
			std::string error_msg = "Hata: Kanal adı belirtilmedi.\n";
			send(client_fd, error_msg.c_str(), error_msg.size(), 0);
			return;
		}
		server.handleNamesCommand(client_fd, target);
	}
	else if (command == "EXIT")
	{
		server.removeClient(client_fd);
	} 
	else {
		std::string unknown_msg = command + "\n";
		send(client_fd, unknown_msg.c_str(), unknown_msg.size(), 0);
	}
}