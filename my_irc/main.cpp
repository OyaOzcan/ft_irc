#include "common.hpp"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Kullanım: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    int port = atoi(argv[1]);
    Server server;

    std::string _password = argv[2];

    server.setup(port, _password);
    server.run();

    return 0;
}
