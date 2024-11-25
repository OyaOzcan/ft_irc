#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Common.hpp"
#include "User.hpp"

class User;

class Command {
public:
    // Gelen mesajı ayrıştırır ve işlem yapılacak komutu döner
    static std::string parseCommand(const std::string& message);

    // Şifre kontrolü
    static bool processPassCommand(const std::string& message, const std::string& password);

    // NICK komutunu işler
    static bool processNickCommand(User& user, const std::string& message, std::string& error_message);

    // USER komutunu işler
    static bool processUserCommand(User& user, const std::string& message, std::string& error_message);

    // Genel komut işlem şablonu
    static void handleCommand(const std::string& command, int client_fd);
};

#endif // COMMAND_HPP
