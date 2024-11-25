#include "../include/Authenticator.hpp"
#include "../include/Numeric.hpp"

Authenticator::Authenticator(const std::string& server_password)
    : _password(server_password) {}

bool Authenticator::authenticate(const std::string& message, std::string& error_message) {
    std::istringstream stream(message);
    std::string line;

    while (std::getline(stream, line)) {
        // Satır sonu karakterlerini temizle
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());

        if (line.find("PASS :") == 0) {
            std::string received_password = line.substr(6);
            if (received_password == _password) {
                return true;
            } else {
                error_message = Numeric::ERR_PASSWDMISMATCH();
                return false;
            }
        }
    }

    error_message = Numeric::ERR_NEEDMOREPARAMS("PASS");
    return false;
}
