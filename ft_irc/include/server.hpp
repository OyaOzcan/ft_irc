#ifndef SERVER_HPP
#define SERVER_HPP

#include "Common.hpp"
#include "Authenticator.hpp"

class Server {
private:
    int _server_fd;
    std::vector<pollfd> _poll_fds;
    std::map<int, bool> _client_authenticated;
    Authenticator _authenticator;

    void setupSocket(int port);
    int createSocket();
    void setSocketNonBlocking(int socket_fd);
    void bindSocket(int socket_fd, int port);
    void startListening(int socket_fd);

    void handleNewClient();
    void handleClientMessage(int client_fd);
    bool isClientFd(const pollfd& p, int client_fd);

public:
    Server(int port, const std::string& password);
    ~Server();

    void run();
};

#endif
