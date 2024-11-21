#include "common.hpp"

Channel::Channel() : name(""), user_limit(-1), invite_only(false) {}

Channel::Channel(const std::string& name) : name(name), user_limit(-1), invite_only(false) {}

Channel::~Channel() {}

const std::string& Channel::getName() const {
    return name;
}

const std::string& Channel::getTopic() const {
    return topic;
}

void Channel::setTopic(const std::string& new_topic) {
    topic = new_topic;
}

void Channel::addMember(int client_fd) {
	members.push_back(client_fd);
}

void Channel::removeMember(int client_fd) {
	std::vector<int>::iterator it = std::find(members.begin(), members.end(), client_fd);
	if (it != members.end()) {
		members.erase(it);
	}
}

void Channel::addOperator(int client_fd) {
	operators.insert(client_fd);
	std::cout << "Operatör eklendi: " << client_fd << std::endl;
}

void Channel::removeOperator(int client_fd) {
	operators.erase(client_fd);
	std::cout << "Operatör silindi: " << client_fd << std::endl;
}

void Channel::broadcastMessage(int sender_fd, const std::string& message) {
	for (size_t i = 0; i < members.size(); ++i) {
		int member_fd = members[i];
		if (member_fd != sender_fd) {
			send(member_fd, message.c_str(), message.size(), 0);
		}
	}
}

bool Channel::isOperator(int client_fd) {
    bool result = operators.find(client_fd) != operators.end();
    return result;
}


void Channel::setInviteOnly(bool value) {
    invite_only = value;
}

void Channel::setPassword(const std::string& pass) {
    password = pass;
}

void Channel::setUserLimit(int limit) {
    user_limit = limit;
}

void Channel::addInvite(int client_fd) {
    invite_list.insert(client_fd);
}

bool Channel::isInvited(int client_fd) const {
    return invite_list.find(client_fd) != invite_list.end();
}

const std::vector<int>& Channel::getMembers() const {
    return members;
}
