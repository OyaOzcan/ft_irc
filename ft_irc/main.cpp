#include "server.hpp"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Kullanım: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    int port = atoi(argv[1]);
    std::string password = argv[2];

    if (port <= 0 || port > 65535) {
        std::cerr << "Hata: Port numarası 1 ile 65535 arasında olmalıdır." << std::endl;
        return 1;
    }

    if (password.empty()) {
        std::cerr << "Hata: Şifre boş olamaz." << std::endl;
        return 1;
    }

    Server server;
    server.setup(port, password);
    server.run();

    return 0;
}