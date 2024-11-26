#include "../include/Channel.hpp"
#include <algorithm> // std::find

Channel::Channel(Server *server, const std::string& name, const std::string& topic, const std::string& password, int user_limit)
    : _server(server), _name(name), _topic(topic), _password(password), _user_limit(user_limit) {
}

Channel::~Channel() {
}

const std::string& Channel::getName() const { return _name; }

const std::string& Channel::getTopic() const { return _topic; }

const std::string& Channel::getPassword() const { return _password; }

int Channel::getUserLimit() const { return _user_limit; }

void Channel::setName(const std::string& name) { _name = name; }

void Channel::setTopic(const std::string& topic) { _topic = topic; }

void Channel::setPassword(const std::string& password) { _password = password; }

void Channel::setUserLimit(int user_limit) { _user_limit = user_limit; }

void Channel::addOperator(User *user) { _operators.push_back(user); }

void Channel::removeOperator(User *user) { _operators.remove(user); }

bool Channel::isOperator(User *user) const {
    return std::find(_operators.begin(), _operators.end(), user) != _operators.end();
}

void Channel::addMember(User *user) {
    if (std::find(_members.begin(), _members.end(), user) == _members.end()) {
        _members.push_back(user);
        std::cout << "User " << user->getNickname() << " added to channel " << _name << std::endl;
    } else {
        std::cout << "User " << user->getNickname() << " is already a member of channel " << _name << std::endl;
    }
}


void Channel::removeMember(User *user) { _members.remove(user); }

bool Channel::isMember(User *user) const {
    return std::find(_members.begin(), _members.end(), user) != _members.end();
}

void Channel::addBannedUser(User *user) { _banned_users.push_back(user); }

void Channel::removeBannedUser(User *user) { _banned_users.remove(user); }

bool Channel::isBannedUser(User *user) const {
    return std::find(_banned_users.begin(), _banned_users.end(), user) != _banned_users.end();
}

void Channel::addInvitedUser(User *user) { _invited_users.push_back(user); }

void Channel::removeInvitedUser(User *user) { _invited_users.remove(user); }

bool Channel::isInvitedUser(User *user) const {
    return std::find(_invited_users.begin(), _invited_users.end(), user) != _invited_users.end();
}

void Channel::sendMessage(User* sender, const std::string& message) {
    std::string full_message = ":" + sender->getNickname() + " PRIVMSG " + _name + " :" + message + "\r\n";

    for (std::list<User*>::iterator it = _members.begin(); it != _members.end(); ++it) {
        User* member = *it;
        std::cout << "Sending to member: " << member->getNickname() << " (fd: " << member->getFd() << ")" << std::endl;
        if (member->getFd() != sender->getFd()) { // Mesajı gönderen hariç diğerlerine gönder
            ssize_t bytes_sent = _server->sendMessage(member->getFd(), full_message);
            if (bytes_sent < 0) {
                std::cerr << "Failed to send message to: " << member->getNickname() << " (fd: " << member->getFd() << ")" << std::endl;
            }
        }
    }
}

void Channel::sendNotice(User *sender, const std::string& notice) {
    std::string full_notice = ":" + sender->getNickname() + " NOTICE " + _name + " :" + notice + "\r\n";
    for (std::list<User*>::iterator it = _members.begin(); it != _members.end(); ++it) {
        _server->sendMessage((*it)->getFd(), full_notice);
    }
}

void Channel::sendAction(User *sender, const std::string& action) {
    std::string full_action = ":" + sender->getNickname() + " PRIVMSG " + _name + " :\001ACTION " + action + "\001\r\n";
    for (std::list<User*>::iterator it = _members.begin(); it != _members.end(); ++it) {
        _server->sendMessage((*it)->getFd(), full_action);
    }
}

void Channel::sendJoin(User *sender) {
    std::string full_join = ":" + sender->getNickname() + " JOIN " + _name + "\r\n";
    for (std::list<User*>::iterator it = _members.begin(); it != _members.end(); ++it) {
        _server->sendMessage((*it)->getFd(), full_join);
    }
}

void Channel::sendPart(User *sender) {
    std::string full_part = ":" + sender->getNickname() + " PART " + _name + "\r\n";
    for (std::list<User*>::iterator it = _members.begin(); it != _members.end(); ++it) {
        _server->sendMessage((*it)->getFd(), full_part);
    }
}

void Channel::sendKick(User *sender, User *target) {
    std::string full_kick = ":" + sender->getNickname() + " KICK " + _name + " " + target->getNickname() + "\r\n";
    for (std::list<User*>::iterator it = _members.begin(); it != _members.end(); ++it) {
        _server->sendMessage((*it)->getFd(), full_kick);
    }
}

void Channel::sendMode(User *sender, const std::string& mode) {
    std::string full_mode = ":" + sender->getNickname() + " MODE " + _name + " " + mode + "\r\n";
    for (std::list<User*>::iterator it = _members.begin(); it != _members.end(); ++it) {
        _server->sendMessage((*it)->getFd(), full_mode);
    }
}

void Channel::sendTopic(User *sender, const std::string& topic) {
    std::string full_topic = ":" + sender->getNickname() + " TOPIC " + _name + " :" + topic + "\r\n";
    for (std::list<User*>::iterator it = _members.begin(); it != _members.end(); ++it) {
        _server->sendMessage((*it)->getFd(), full_topic);
    }
}

void Channel::sendNames(User* sender) {
    std::cout << "sendNames called for channel: " << _name << std::endl;
    std::string full_names = ":" + sender->getNickname() + " 353 " + sender->getNickname() + " = " + _name + " :";

    // std::list için explicit iterator kullanımı
    std::list<User*>::iterator it;
    for (it = _members.begin(); it != _members.end(); ++it) {
        User* member = *it;
        std::cout << "Member: " << member->getNickname() << " fd: " << member->getFd() << std::endl;
        full_names += member->getNickname() + " ";
    }

    full_names += "\r\n";
    _server->sendMessage(sender->getFd(), full_names);

    // Kanal sonu bildirimi
    std::string end_of_names = ":" + sender->getNickname() + " 366 " + sender->getNickname() + " " + _name + " :End of /NAMES list\r\n";
    _server->sendMessage(sender->getFd(), end_of_names);
}

