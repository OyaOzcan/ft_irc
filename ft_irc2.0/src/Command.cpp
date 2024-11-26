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
bool Command::processPassCommand(const std::string& message, const std::string& expected_password) {
    // Komutun "PASS " ile başladığını kontrol edin
    if (message.find("PASS :") != 0) {
        return false; // Format hatası
    }

    // Şifre kısmını alın ve temizleyin
    std::string received_password = message.substr(6); // "PASS " kısmını atla
    received_password.erase(received_password.find_last_not_of("\r\n") + 1); // Fazlalıkları temizle
    //received_password = received_password.substr(received_password.find_first_not_of(":")); // ":"'yi çıkar

    if (received_password != expected_password) {
        return false; // Şifre eşleşmedi
    }

    return true; // Şifre doğru
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

bool Command::processNickCommand(User& user, const std::string& message, std::string& error_message) {
    if (message.find("NICK ") != 0) {
        error_message = "ERR_NEEDMOREPARAMS :NICK\n";
        return false;
    }

    std::string nickname = message.substr(5);
    nickname.erase(std::remove(nickname.begin(), nickname.end(), '\r'), nickname.end());
    nickname.erase(std::remove(nickname.begin(), nickname.end(), '\n'), nickname.end());

    if (nickname.empty()) {
        error_message = "ERR_NEEDMOREPARAMS :NICK\n";
        return false;
    }

    // TODO: Mevcut tüm kullanıcıların nicknames kontrol edilerek "ERR_NICKNAMEINUSE" döndürülebilir
    user.setNickname(nickname);
    return true;
}

// USER komutunu işler
bool Command::processUserCommand(User& user, const std::string& message, std::string& error_message) {
    if (message.find("USER ") != 0) {
        error_message = "ERR_NEEDMOREPARAMS :USER\n";
        return false;
    }

    std::istringstream stream(message.substr(5));
    std::string username, hostname;

    if (!(stream >> username >> hostname)) {
        error_message = "ERR_NEEDMOREPARAMS :USER\n";
        return false;
    }

    user.setUsername(username);
    user.setHostname(hostname);
    return true;
}

