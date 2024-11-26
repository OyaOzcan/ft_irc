#ifndef USER_HPP
#define USER_HPP

#include <string>

class User {
private:
    std::string _nickname;
    std::string _username;

public:
    User();
    User(const std::string& nickname, const std::string& username);
    ~User();

    const std::string& getNickname() const;
    const std::string& getUsername() const;

    void setNickname(const std::string& nickname);
    void setUsername(const std::string& username);

    bool isFullyRegistered() const;
};

#endif
