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

        //setSocketNonBlocking(client_fd);

        pollfd client_poll_fd = {client_fd, POLLIN, 0};
        _poll_fds.push_back(client_poll_fd);
        _client_authenticated[client_fd] = false;

        std::cout << "New client connected: " << client_fd << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error in handleNewClient: " << ex.what() << std::endl;
    }
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
