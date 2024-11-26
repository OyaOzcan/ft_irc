#ifndef AUTHENTICATOR_HPP
#define AUTHENTICATOR_HPP

#include "Common.hpp"

class Authenticator {
private:
    std::string _password;
public:
    Authenticator(const std::string& server_password);

    bool authenticate(const std::string& message, std::string& error_message);
};

#endif
