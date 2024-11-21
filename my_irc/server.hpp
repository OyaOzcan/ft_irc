#ifndef SERVER_HPP
#define SERVER_HPP

#include "common.hpp"
#include <map>
#include <vector>
#include <string>

class Server {
private:
    int server_fd;
    std::vector<pollfd> poll_fds;
    std::string password; // Doğru şifre
    std::map<int, bool> client_authenticated; // İstemcilerin doğrulanma durumu

public:
    Server();
    ~Server();

    void setup(int port, std::string _password);
    void run();
};

#endif // SERVER_HPP
