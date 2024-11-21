#ifndef HANDLE_HPP
#define HANDLE_HPP

#include "common.hpp"

// Sunucu sınıfı
void handleClientMessages(int client_fd, const std::string& message, Server& server);

#endif
