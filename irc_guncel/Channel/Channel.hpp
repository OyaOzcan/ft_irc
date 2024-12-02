/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgunay <fgunay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:19:23 by fgunay            #+#    #+#             */
/*   Updated: 2024/12/02 17:20:16 by fgunay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

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

#include "../Client/Client.hpp"
class Client;

class Channel{
    private:
        std::string topic;
        std::string name;
        std::vector<Client> Operators;
        std::vector<Client> members;
        std::string key;
        bool required_key;
        bool invite_only;
    public:
        Channel();
        Channel(std::string name);
        Channel(std::string name, std::string key);
        Channel(Channel const &client);
        void    operator=(Channel const &client);
        ~Channel();
        
        void    addOperators(Client const &client);
        void    addMembers(Client const &client);

        std::string getName(void);
        std::vector<Client> &getMembers(void);
        std::vector<Client> &getOperators(void);
        std::string &getKey(void);
        std::string &getTopic(void);
        bool getRequiredKey();
        bool getInviteOnly();
        bool isMember(Client const &client);
        bool isOperator(Client const &client);
        void printMembers();

        void inviteOnly(char command, Client &client);
        void operators(char command, Client &operators, Client &client);
        void keys(char command, std::string key, Client &client);
        
        void    setTopic(std::string topic);
        int getOperatorIndex(std::string nick);
};

#endif