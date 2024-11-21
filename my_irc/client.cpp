#include "common.hpp"

void runClient() {
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Soket oluşturulamadı");
        exit(1);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    if (connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Bağlantı hatası");
        close(client_fd);
        exit(1);
    }

    std::cout << "Sunucuya bağlandı. Mesaj gönderin ('exit' yazarak çıkabilirsiniz)." << std::endl;

    char buffer[1024] = {0};
    while (true) {
        std::string message;
        std::cout << "Mesaj yaz: ";
        std::getline(std::cin, message);

        if (message == "exit") {
            std::cout << "Bağlantı sonlandırılıyor...\n";
            close(client_fd);
            break;
        }
        if (send(client_fd, message.c_str(), message.size(), 0) < 0) {
            perror("Mesaj gönderilemedi");
            break;
        }


        memset(buffer, 0, sizeof(buffer));
        int valread = recv(client_fd, buffer, sizeof(buffer), 0);
        if (valread > 0) {
            std::cout << "Sunucudan mesaj: " << buffer << std::endl;
        } else if (valread == 0) {
            std::cout << "Sunucu bağlantıyı kapattı." << std::endl;
            break;
        } else {
            perror("recv hatası");
            break;
        }
    }

    close(client_fd);
}
