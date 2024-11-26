#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include <list>
# include <iostream>
# include "User.hpp"
# include "Server.hpp"

class Server;
class User;

class Channel {
    private:
        Server      *_server;
        std::string _name;
        std::string _topic;
        std::string _password;
        int         _user_limit;

        std::list<User*> _operators;
        std::list<User*> _members;
        std::list<User*> _banned_users;
        std::list<User*> _invited_users;
    
    public:
        Channel(Server *server, const std::string& name, const std::string& topic, const std::string& password, int user_limit);
        ~Channel();

        const std::string& getName() const;
        const std::string& getTopic() const;
        const std::string& getPassword() const;
        int getUserLimit() const;

        void setName(const std::string& name);
        void setTopic(const std::string& topic);
        void setPassword(const std::string& password);
        void setUserLimit(int user_limit);

        void addOperator(User *user);
        void removeOperator(User *user);
        bool isOperator(User *user) const;

        void addMember(User *user);
        void removeMember(User *user);
        bool isMember(User *user) const;

        void addBannedUser(User *user);
        void removeBannedUser(User *user);
        bool isBannedUser(User *user) const;

        void addInvitedUser(User *user);
        void removeInvitedUser(User *user);
        bool isInvitedUser(User *user) const;

        void sendMessage(User *sender, const std::string& message);
        void sendNotice(User *sender, const std::string& notice);
        void sendAction(User *sender, const std::string& action);
        void sendJoin(User *sender);
        void sendPart(User *sender);
        void sendKick(User *sender, User *target);
        void sendMode(User *sender, const std::string& mode);
        void sendTopic(User *sender, const std::string& topic);
        void sendNames(User *sender);
        void sendList(User *sender);
        void sendInvite(User *sender, User *target);
        void sendWho(User *sender);
        void sendWhois(User *sender, User *target);
        void sendWhowas(User *sender, User *target);

        void sendChannelMessage(User *sender, const std::string& message);
        void sendChannelNotice(User *sender, const std::string& notice);
        void sendChannelAction(User *sender, const std::string& action);
        void sendChannelJoin(User *sender);
        void sendChannelPart(User *sender);
        void sendChannelKick(User *sender, User *target);
        void sendChannelMode(User *sender, const std::string& mode);
        void sendChannelTopic(User *sender, const std::string& topic);
        void sendChannelNames(User *sender);
        void sendChannelList(User *sender);
        void sendChannelInvite(User *sender, User *target);
        void sendChannelWho(User *sender);
        void sendChannelWhois(User *sender, User *target);

        void sendChannelMessage(const std::string& message);
        void sendChannelNotice(const std::string& notice);
        void sendChannelAction(const std::string& action);
        void sendChannelJoin();
        void sendChannelPart();
        void sendChannelKick(User *target);
        void sendChannelMode(const std::string& mode);
        void sendChannelTopic(const std::string& topic);
        void sendChannelNames();
        void sendChannelList();
        void sendChannelInvite(User *target);
        void sendChannelWho();
        void sendChannelWhois(User *target);
};

#endif // CHANNEL_HPP