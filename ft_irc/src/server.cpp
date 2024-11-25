#include "Server.hpp"

bool Server::isClientFd(const pollfd& p, int client_fd) {
    return p.fd == client_fd;
}

Server::Server(int port, const std::string& password) 
    : _server_fd(-1), _authenticator(password) {
    setupSocket(port);
}

Server::~Server() {
    if (_server_fd != -1) {
        close(_server_fd);
    }
}

void Server::setupSocket(int port) {
    _server_fd = createSocket();
    setSocketNonBlocking(_server_fd);
    bindSocket(_server_fd, port);
    startListening(_server_fd);

    pollfd server_poll_fd = {_server_fd, POLLIN, 0};
    _poll_fds.push_back(server_poll_fd);
}

int Server::createSocket() {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        throw std::runtime_error("Socket creation failed");
    }
    return socket_fd;
}

void Server::setSocketNonBlocking(int socket_fd) {
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

        setSocketNonBlocking(client_fd);

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
        std::string message(buffer);
        if (!_client_authenticated[client_fd]) {
            std::string error_message;
            if (_authenticator.authenticate(message, error_message)) {
                _client_authenticated[client_fd] = true;
                std::string success_message = "Authentication successful.\n";
                send(client_fd, success_message.c_str(), success_message.size(), 0);
            } else {
                std::cerr << "Authentication failed: " << error_message << std::endl;
                std::string failure_message = error_message + "\n";
                send(client_fd, failure_message.c_str(), failure_message.size(), 0);
                close(client_fd);

                for (size_t i = 0; i < _poll_fds.size(); ++i) {
                    if (isClientFd(_poll_fds[i], client_fd)) {
                        _poll_fds.erase(_poll_fds.begin() + i);
                        break;
                    }
                }

                _client_authenticated.erase(client_fd);
            }
        } else {
            std::cout << "Message from authenticated client " << client_fd << ": " << message << std::endl;
        }
    } else {
        close(client_fd);

        for (size_t i = 0; i < _poll_fds.size(); ++i) {
            if (isClientFd(_poll_fds[i], client_fd)) {
                _poll_fds.erase(_poll_fds.begin() + i);
                break;
            }
        }

        _client_authenticated.erase(client_fd);
    }
}
