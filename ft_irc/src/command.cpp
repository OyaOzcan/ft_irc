#include "Command.hpp"

// Mesajdan komutu ayıklar (ör. "PASS :1234" -> "PASS")
std::string Command::parseCommand(const std::string& message) {
    size_t spacePos = message.find(' ');
    if (spacePos != std::string::npos) {
        return message.substr(0, spacePos);
    }
    return message; // Komut tek kelime ise
}

// Şifre kontrolünü gerçekleştirir
bool Command::processPassCommand(const std::string& message, const std::string& password) {
    if (message.find("PASS :") == 0) {
        std::string received_password = message.substr(6); // "PASS :" kısmını atla
        received_password.erase(std::remove(received_password.begin(), received_password.end(), '\r'), received_password.end());
        received_password.erase(std::remove(received_password.begin(), received_password.end(), '\n'), received_password.end());
        return received_password == password;
    }
    return false;
}

// Komutları işler
void Command::handleCommand(const std::string& command, int client_fd) {
    if (command == "PING") {
        std::string pong_response = "PONG :127.0.0.1\r\n";
        send(client_fd, pong_response.c_str(), pong_response.size(), 0);
        std::cout << "PONG gönderildi." << std::endl;
    } else if (command == "CAP") {
        // CAP komutlarını göz ardı et
        std::cout << "CAP komutu atlandı." << std::endl;
    } else {
        // Bilinmeyen komutlar
        std::cout << "Bilinmeyen komut: " << command << std::endl;
    }
}