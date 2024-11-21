#include "common.hpp"

Server::Server() : server_fd(-1) {}

Server::~Server() {
    if (server_fd != -1) close(server_fd);
}

void Server::setup(int port, std::string _password) {
    struct sockaddr_in address;
    password = _password; // Şifreyi sakla

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
                client_authenticated[client_fd] = false; // İstemci ilk başta doğrulanmamış
            } else if (poll_fds[i].revents & POLLIN) {
                char buffer[1024] = {0};
                int bytes_read = recv(poll_fds[i].fd, buffer, sizeof(buffer), 0);
                if (bytes_read > 0) {
                    std::string message(buffer);
                    std::cout << "İstemci " << poll_fds[i].fd << " mesaj gönderdi: " << message << std::endl;

                    // Şifre kontrolü
                    if (!client_authenticated[poll_fds[i].fd]) {
                         client_authenticated[poll_fds[i].fd] = true;
                            std::string success_message = "Doğrulama başarılı.\n";
                            send(poll_fds[i].fd, success_message.c_str(), success_message.size(), 0);
                            std::cout << "İstemci " << poll_fds[i].fd << " başarıyla doğrulandı." << std::endl;
                        /*if (message == password) {
                            client_authenticated[poll_fds[i].fd] = true;
                            std::string success_message = "Doğrulama başarılı.\n";
                            send(poll_fds[i].fd, success_message.c_str(), success_message.size(), 0);
                            std::cout << "İstemci " << poll_fds[i].fd << " başarıyla doğrulandı." << std::endl;
                        } else {
                            std::string failure_message = "Hatalı şifre. Bağlantı kesiliyor.\n";
                            send(poll_fds[i].fd, failure_message.c_str(), failure_message.size(), 0);
                            close(poll_fds[i].fd);
                            poll_fds.erase(poll_fds.begin() + i);
                            client_authenticated.erase(poll_fds[i].fd);
                            --i;
                        }*/
                    }
                } else {
                    close(poll_fds[i].fd);
                    poll_fds.erase(poll_fds.begin() + i);
                    client_authenticated.erase(poll_fds[i].fd);
                    --i;
                }
            }
        }
    }
}
