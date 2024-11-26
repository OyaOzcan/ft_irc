#include "Common.hpp"
#include "User.hpp"
#include "Command.hpp"

bool Server::isClientFd(const pollfd& p, int client_fd) {
    return p.fd == client_fd;
}

Server::Server(int port, const std::string& password)
    : _authenticator(password) {
    // Sunucu kurulum işlemleri burada yapılır
    setupSocket(port);
    std::cout << "Server started on port " << port << " with password: " << password << std::endl;
}


Server::~Server() {
    if (_server_fd != -1) {
        close(_server_fd);
    }
}

void Server::setupSocket(int port) {
    _server_fd = createSocket(); // socket_fd oluşturuldu.
    setSocketNonBlocking(_server_fd); // socket_fd non-blocking yapıldı.
    bindSocket(_server_fd, port);
    startListening(_server_fd);

    pollfd server_poll_fd = {_server_fd, POLLIN, 0};
    _poll_fds.push_back(server_poll_fd);
}

int Server::createSocket() {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4 , SOCK_STREAM = "Bağlantı Türü tipi" TCP, 0 = "Protokol" (IP)
    if (socket_fd < 0) {
        throw std::runtime_error("Socket creation failed");
    }
    return socket_fd;
}

void Server::setSocketNonBlocking(int socket_fd) {
    // TODO : linux için değiştirmek lazım.
    if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) < 0) {
        close(socket_fd);
        throw std::runtime_error("Failed to set socket to non-blocking");
    }
}

void Server::bindSocket(int socket_fd, int port) {
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int opt = 1;
if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt failed");
    exit(EXIT_FAILURE);
}

    if (bind(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        close(socket_fd);
        throw std::runtime_error("Bind failed");
    }
}

void Server::startListening(int socket_fd) {
    if (listen(socket_fd, 10) < 0) {
        close(socket_fd);
        throw std::runtime_error("Listen failed");
    }
}

void Server::run() {
    try {
        while (true) {
            int poll_count = poll(&_poll_fds[0], _poll_fds.size(), -1);
            if (poll_count < 0) {
                throw std::runtime_error("Poll error");
            }

            for (size_t i = 0; i < _poll_fds.size(); ++i) {
                if (_poll_fds[i].fd == _server_fd && (_poll_fds[i].revents & POLLIN)) {
                    handleNewClient();
                } else if (_poll_fds[i].revents & POLLIN) {
                    handleClientMessage(_poll_fds[i].fd);
                }
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "Server error: " << ex.what() << std::endl;
    }
}

void Server::handleNewClient() {
    try {
        int client_fd = accept(_server_fd, NULL, NULL);
        if (client_fd < 0) {
            throw std::runtime_error("Accept failed");
        }

        pollfd client_poll_fd = {client_fd, POLLIN, 0};
        _poll_fds.push_back(client_poll_fd);

        // Kullanıcı oluştur ve fd değerini ata
        User new_user;
        new_user.setFd(client_fd); // Yeni eklenen setFd çağrısı
        _clients[client_fd] = new_user;

        std::cout << "New client connected: " << client_fd << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error in handleNewClient: " << ex.what() << std::endl;
    }
}


ssize_t Server::sendMessage(int client_fd, const std::string& message) {
    ssize_t bytes_sent = send(client_fd, message.c_str(), message.size(), 0);
    if (bytes_sent < 0) {
        perror("send failed");
    }
    return bytes_sent;
}




void Server::handleClientMessage(int client_fd) {
    char buffer[1024] = {0};
    int bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);

    if (bytes_read > 0) {
        std::string raw_message(buffer);

        // Gelen mesajı satırlara böl
        std::istringstream message_stream(raw_message);
        std::string line;

        while (std::getline(message_stream, line)) {

            //line.erase(line.find_last_not_of("\r") + 1); // Fazla '\r' karakterlerini temizle

            std::cout << "\n-----------------------\n";
            std::cout << "Processing command: " << line << "\n";
            std::cout << "-----------------------\n";

            std::string command = Command::parseCommand(line);
            std::string error_message;
            User& user = _clients[client_fd];
            if (command == "CREATE") {
                std::string channelName = Command::getChannelName(line);
                std::string topic = Command::getTopic(line);
                try {
                    createChannel(channelName, topic);
                    send(client_fd, "Channel created successfully\r\n", 31, 0);
                } catch (const std::exception& e) {
                    send(client_fd, e.what(), strlen(e.what()), 0);
                }
            } else if (command == "JOIN") {
                std::string channelName = Command::getChannelName(line);
                Channel* channel = getChannel(channelName);

                if (channel) {
                    if (!channel->isMember(&user)) {
                        channel->addMember(&user);  // Kullanıcıyı kanala ekle
                        channel->sendJoin(&user);  // Diğer üyelere katılım bildirimi gönder
                        send(client_fd, ("Joined channel: " + channelName + "\r\n").c_str(), 31, 0);
                        std::cout << user.getNickname() << " joined channel: " << channelName << std::endl;
                    } else {
                        send(client_fd, "You are already in the channel\r\n", 33, 0);
                    }
                } else {
                    send(client_fd, "No such channel exists\r\n", 25, 0);
                }
            }

            if (command == "NAMES") {
                std::string channelName = Command::getChannelName(line);
                Channel* channel = getChannel(channelName);
                if (channel) {
                    channel->sendNames(&user); // Kanal içindeki kullanıcıları gönder
                } else {
                    send(client_fd, "No such channel exists\r\n", 25, 0);
                }
            }
            if (command == "PRIVMSG") {
                size_t first_space = line.find(' ');
                if (first_space == std::string::npos) {
                    std::string error_message = Numeric::ERR_NEEDMOREPARAMS("PRIVMSG") + "\r\n";
                    send(client_fd, error_message.c_str(), error_message.size(), 0);
                    return;
                }

                size_t second_space = line.find(' ', first_space + 1);
                if (second_space == std::string::npos) {
                    std::string error_message = Numeric::ERR_NEEDMOREPARAMS("PRIVMSG") + "\r\n";
                    send(client_fd, error_message.c_str(), error_message.size(), 0);
                    return;
                }

                std::string target = line.substr(first_space + 1, second_space - first_space - 1);
                std::string message = line.substr(second_space + 2); // ':' işaretinden sonra mesaj

                std::cout << "Target: " << target << ", Message: " << message << std::endl;

                if (target[0] == '#') { // Kanal mesajı
                    Channel* channel = getChannel(target.substr(1)); // '#' sonrası kanal adı
                    if (channel) {
                        channel->sendMessage(&user, message); // Kanal üyelerine mesaj gönder
                    } else {
                        std::string error_message = Numeric::ERR_NOSUCHCHANNEL(target) + "\r\n";
                        send(client_fd, error_message.c_str(), error_message.size(), 0);
                    }
                } else {
                    std::string error_message = Numeric::ERR_NOSUCHNICK(target) + "\r\n";
                    send(client_fd, error_message.c_str(), error_message.size(), 0);
                }
            }



            switch (user.getState()) {
            case NOT_AUTHENTICATED:
                if (command == "PASS") {
                    std::cout << "PASS command received.\n";
                    if (Command::processPassCommand(line, _authenticator.getPassword())) {
                        user.setState(WAITING_FOR_NICK);
						if (line.find("\r") != std::string::npos) {
							std::string welcome_message = Numeric::RPL_WELCOME(user.getNickname(), user.getUsername(), user.getHostname()) + "\r\n";
							Command::processNickCommand(user, line, welcome_message);
                        	//send(client_fd, welcome_message.c_str(), welcome_message.size(), 0);
                        	//send(client_fd, "PASS accepted. Please provide NICK1.\r\n\n", 39, 0);
						}else {
							send(client_fd, "PASS accepted. Please provide NICK.\r\n", 39, 0);
						}
                        std::cout << "Password authentication successful.\n";
                    } else {
                        send(client_fd, "ERR_PASSWDMISMATCH :Password incorrect\r\n", 41, 0);
                        std::cout << "Password authentication failed.\n";
                    }
                } else {
                    send(client_fd, "ERR_NOTREGISTERED :You must send PASS first\r\n", 45, 0);
                }
                break;

            case WAITING_FOR_NICK:
                if (command == "NICK") {
                    std::cout << "NICK command received.\n";
                    if (Command::processNickCommand(user, line, error_message)) {
                        user.setState(WAITING_FOR_USER);
                        send(client_fd, "NICK accepted. Please provide USER.\r\n", 38, 0);
                        std::cout << "Nickname set: " << user.getNickname() << "\n";
                    } else {
                        send(client_fd, error_message.c_str(), error_message.size(), 0);
                        std::cout << "Nickname error: " << error_message << "\n";
                    }
                } else {
                    send(client_fd, "ERR_NEEDMOREPARAMS :NICK required\r\n", 35, 0);
                }
                break;

            case WAITING_FOR_USER:
                if (command == "USER") {
                    std::cout << "USER command received.\n";
                    if (Command::processUserCommand(user, line, error_message)) {
                        user.setState(FULLY_REGISTERED);
                        std::string welcome_message = Numeric::RPL_WELCOME(user.getNickname(), user.getUsername(), user.getHostname()) + "\r\n";
                        send(client_fd, welcome_message.c_str(), welcome_message.size(), 0);
                        std::cout << "User registration complete for: " << user.getNickname() << "\n";
                    } else {
                        send(client_fd, error_message.c_str(), error_message.size(), 0);
                        std::cout << "User registration error: " << error_message << "\n";
                    }
                } else {
                    send(client_fd, "ERR_NEEDMOREPARAMS :USER required\r\n", 35, 0);
                }
                break;

            case FULLY_REGISTERED:
                std::cout << "Message from " << user.getNickname() << ": " << line << "\n";
                break;
                

            default:
                send(client_fd, "ERR_UNKNOWNCOMMAND :Unknown command\r\n", 38, 0);
                break;
            }
        }
    } else {
        close(client_fd);
        _clients.erase(client_fd);
        std::cout << "Client disconnected: " << client_fd << "\n";
    }
}


// CHANNEL İŞLEMLERİ

void Server::createChannel(const std::string& channelName, const std::string& topic,
                           const std::string& password, int userLimit) {
    if (_channels.find(channelName) != _channels.end()) {
        throw std::runtime_error("Channel already exists: " + channelName);
    }
    _channels[channelName] = new Channel(this, channelName, topic, password, userLimit);
    std::cout << "Channel created: " << channelName << std::endl;
}

Channel* Server::getChannel(const std::string& channelName) {
    std::cout << "getChannel called for: " << channelName << std::endl;
    if (_channels.find(channelName) == _channels.end()) {
        std::cout << "Channel not found: " << channelName << std::endl;
        return NULL; // Kanal bulunamadı
    }
    std::cout << "Channel found: " << channelName << std::endl;
    return _channels[channelName];
}



void Server::deleteChannel(const std::string& channelName) {
    std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
    if (it != _channels.end()) {
        delete it->second; // Kanal belleğini serbest bırak
        _channels.erase(it);
        std::cout << "Channel deleted: " << channelName << std::endl;
    } else {
        std::cerr << "Attempted to delete a non-existent channel: " << channelName << std::endl;
    }
}