#ifndef USER_HPP
#define USER_HPP

#include "Common.hpp"
#include "Command.hpp"
#include "Server.hpp"
#include "Authenticator.hpp"
#include "Numeric.hpp"
#include "User.hpp"

enum ClientState {
    NOT_AUTHENTICATED,
    WAITING_FOR_NICK,
    WAITING_FOR_USER,
    FULLY_REGISTERED
};

class User {
private:
    std::string _nickname;
    std::string _username;
    std::string _hostname;
    //bool _has_nickname;
   // bool _has_username;
    ClientState _state; // Kullanıcının durumu
   // bool _is_registered;

public:
    User();
    User(const std::string& nickname, const std::string& username, const std::string& hostname);

    ~User();

    const std::string& getNickname() const;
    const std::string& getUsername() const;
    const std::string& getHostname() const;

    void setNickname(const std::string& nickname);
    void setUsername(const std::string& username);
    void setHostname(const std::string& hostname);

    ClientState getState() const;
    void setState(ClientState state);

    bool isFullyRegistered() const;
};

#endif // USER_HPP
