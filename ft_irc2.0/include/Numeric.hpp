#ifndef NUMERIC_HPP
#define NUMERIC_HPP

#include "Common.hpp"
#include "User.hpp"
#include "Command.hpp"
#include "Server.hpp"

class Numeric {
public:
    static std::string ERR_PASSWDMISMATCH() {
        return "464 :Password incorrect";
    }

    static std::string ERR_NEEDMOREPARAMS(const std::string& command) {
        return "461 " + command + " :Not enough parameters";
    }

    static std::string RPL_WELCOME(const std::string& nick, const std::string& user, const std::string& host) {
        return "001 " + nick + " :Welcome to the Internet Relay Network, " + nick + "!" + user + "@" + host;
    }

    static std::string ERR_UNKNOWNCOMMAND(const std::string& command) {
        return "421 " + command + " :Unknown command";
    }

    static std::string ERR_NONICKNAMEGIVEN() {
        return "431 :No nickname given";
    }

    static std::string ERR_NOSUCHCHANNEL(const std::string& channel) {
        return "403 " + channel + " :No such channel";
    }

    static std::string ERR_NOSUCHNICK(const std::string& nick) {
        return "401 " + nick + " :No such nick/channel";
    }
};

#endif
