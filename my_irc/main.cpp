#include "common.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Kullanım: ./ircserv <port>" << std::endl;
        return 1;
    }

    int port = atoi(argv[1]);
    Server server;

    server.setup(port);
    server.run();

    return 0;
}



/*
struct sockaddr_in {
    sa_family_t    sin_family;   // Adres ailesi (AF_INET - IPv4)
    in_port_t      sin_port;     // Port numarası (htons ile dönüştürülmüş)
    struct in_addr sin_addr;     // IP adresi
    char           sin_zero[8];  // Dolgu (padding, kullanılmaz)
};
*/

/*
struct sockaddr {
    sa_family_t sa_family;   // Adres ailesi (ör. AF_INET, AF_INET6)
    char        sa_data[14]; // Adres verisi
};

*/
