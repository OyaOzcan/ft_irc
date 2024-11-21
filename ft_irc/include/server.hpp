#ifndef SERVER_HPP
#define SERVER_HPP

#define MAX_CLIENTS 1024

#include "common.hpp"

class Server {
private:
    int _socket;
    unsigned _port;
    std::string  _password;

public:
    Server();
    ~Server();

    void setup(unsigned port);

};

#endif
