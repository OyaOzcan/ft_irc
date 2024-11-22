#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <algorithm>
#include <cstring>

class Server {
public:
    Server();
    ~Server();

    void setup(int port, std::string _password);
    void run();

private:
    int server_fd;
    std::string password;
    std::vector<pollfd> poll_fds;
    std::unordered_map<int, bool> client_authenticated;

    bool checkPassword(const std::string& message, int client_fd);
};

#endif // SERVER_HPP
