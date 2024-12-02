/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgunay <fgunay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:20:55 by fgunay            #+#    #+#             */
/*   Updated: 2024/12/02 17:20:56 by fgunay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include <poll.h>

#include "../Server/Server.hpp"
class Server;

class Client
{
    protected:
        int socketID;
        std::string realName;

        std::string nickName;
        std::string userName;
        std::string realIp;

        int fd;
        bool isAuthenticated;
        bool isConnected;
        std::vector<std::string> hasInvitationTo;
    public:
        Client();
        void operator=(Client const &copy);
        Client(Client const &copy);
        
        void setSocket(const int socket);
        void setRealName(const std::string &realName);
        void setNickName(const std::string &nickName);
        void setUserName(const std::string &userName);
        void setRealIp(const std::string &realIp);
        void setFd(int fd);
        void setLoggin(bool status);
        
        std::string &getRealIp();
        
        ~Client();
        Client(int fd);
        bool getLoggedStatus();
        void print(std::string str);
        void clientInfo(Client &receive, Client &client);
        
        int getFd();
        int getSocket();
        std::string getNick() const;
        std::string getUsername();
        std::string getRealName();
        bool getisConnected();
        void setisConnected(bool status);
        bool isChannelInvited(std::string  channel);
        void setInvitationForChannel(std::string channel);

};

#endif