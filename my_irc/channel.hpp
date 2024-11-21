#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "common.hpp"

class Channel {
private:
    std::string name;
    std::string topic;
    std::set<int> operators; // Operatörlerin listesi
    std::vector<int> members;
    std::set<int> invite_list; // Davet edilen kullanıcılar
    int user_limit;
    bool invite_only;
    std::string password;

public:
    Channel();
    Channel(const std::string& name);
    ~Channel();

    const std::string& getName() const;
    const std::string& getTopic() const;
    void setTopic(const std::string& new_topic);

    void addMember(int client_fd);
    void removeMember(int client_fd);
    void addOperator(int client_fd);
    void removeOperator(int client_fd);
    bool isOperator(int client_fd) const;

    void broadcastMessage(int sender_fd, const std::string& message);

    // Kanal modları
    void setInviteOnly(bool value);
    void setPassword(const std::string& pass);
    void setUserLimit(int limit);
    void addInvite(int client_fd);
    bool isInvited(int client_fd) const;
    bool isOperator(int client_fd);

    const std::vector<int>& getMembers() const;
};

#endif

