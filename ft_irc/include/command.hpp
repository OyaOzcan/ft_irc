#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "common.hpp"

class Command {
public:
    // Gelen mesajı ayrıştırır ve işlem yapılacak komutu döner
    static std::string parseCommand(const std::string& message);

    // Şifre kontrolü
    static bool processPassCommand(const std::string& message, const std::string& password);

    // Diğer komutlar için genel işlem şablonu
    static void handleCommand(const std::string& command, int client_fd);
};

#endif // COMMAND_HPP