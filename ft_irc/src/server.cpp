
#include "../include/server.hpp"

Server::Server() : _socket(-1) {}

Server::~Server() {
    if (_socket != -1) close(_socket);
}

void Server::setup(unsigned port) {
    struct sockaddr_in address;

    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0) {
        perror("Socket oluşturulamadı");
        exit(1);
    }

    fcntl(_socket, F_SETFL, O_NONBLOCK);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind hatası");
        close(_socket);
        exit(1);
    }

    if (listen(_socket, 10) < 0) {
        perror("Listen hatası");
        close(_socket);
        exit(1);
    }

    std::cout << "Sunucu başlatıldı: " << port << std::endl;

    int client_fd = accept(_socket, NULL, NULL);
    if (client_fd < 0) {
        perror("Accept hatası");
        return;
    }

    std::cout << "Yeni bir istemci bağlandı: " << client_fd << std::endl;
}