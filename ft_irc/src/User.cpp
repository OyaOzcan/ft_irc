#include "../include/User.hpp"

User::User() : _nickname(""), _username("") {}

User::User(const std::string& nickname, const std::string& username)
    : _nickname(nickname), _username(username) {}

User::~User() {}

const std::string& User::getNickname() const {
    return _nickname;
}

const std::string& User::getUsername() const {
    return _username;
}

void User::setNickname(const std::string& nickname) {
    _nickname = nickname;
}

void User::setUsername(const std::string& username) {
    _username = username;
}

bool User::isFullyRegistered() const {
    return !_nickname.empty() && !_username.empty();
}
