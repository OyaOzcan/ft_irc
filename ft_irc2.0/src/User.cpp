#include "../include/User.hpp"

User::User() : _nickname(""), _username(""), _hostname(""), _state(NOT_AUTHENTICATED) {}

User::User(const std::string& nickname, const std::string& username, const std::string& hostname)
    : _nickname(nickname), _username(username), _hostname(hostname), _state(NOT_AUTHENTICATED) {}

User::~User() {}

const std::string& User::getNickname() const { return _nickname; }
const std::string& User::getUsername() const { return _username; }
const std::string& User::getHostname() const { return _hostname; }

void User::setNickname(const std::string& nickname) { _nickname = nickname; }
void User::setUsername(const std::string& username) { _username = username; }
void User::setHostname(const std::string& hostname) { _hostname = hostname; }

ClientState User::getState() const { return _state; }
void User::setState(ClientState state) { _state = state; }

bool User::isFullyRegistered() const { return _state == FULLY_REGISTERED; }
