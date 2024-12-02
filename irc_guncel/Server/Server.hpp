/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgunay <fgunay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:21:42 by fgunay            #+#    #+#             */
/*   Updated: 2024/12/02 17:21:43 by fgunay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sstream>
#include <string>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include <poll.h>

#include "../Client/Client.hpp"
#include "../Channel/Channel.hpp" 
class Client;
class Channel;

class Server
{
    protected:
        std::vector<struct pollfd >  fds;
        int socketfd;
        int port;
        std::string password;
        int acceptfd;

        sockaddr_in server_address;
        int serverfd;
        int optval;
        socklen_t address_len;
        int acc_value;
        std::string serverIP;

        std::vector<Client> clients;
        std::vector<Channel> channels;
        std::vector<std::string> commands;
    public:
        Server();
        Server(Server const &Server);
        void    operator=(Server const &Server);
        Server(int port, std::string password);
        ~Server();

        void runServer();
        void setUpServerSocket(int server_fd);
        void checkBindStatus(int bind_value);
        void checkListenStatus(int list_value);
        void checkAcceptStatus(int accept_value);
        void acceptClient();
        void checkPollStatus(int poll_status);
        int getAccVal();

        void parseMessage(char *buffer);
        void executeCommands(int fd);
        void PASS(Client &client);
        void NICK(Client &client);
        void USER(Client &client);
        void MODE(Client &client);

        void JOIN(Client &client);
        void INVITE(Client &client);
        void PART(Client &client);
        void TOPIC(Client &client);

        void QUIT(Client &client);
        void WHO(Client &client);
        void PRIVMSG(Client &client);
        void KICK(Client &client);
        void LIST(Client &client);
        

        int checkActivation(Client &client);
        void start();
        int checkRecvStatus(int recv_value, int i);

        bool isChannelExist(std::string channel);
        size_t getClientIndex(std::string clientNick, Channel &channel);
        size_t getChannelIndex(std::string channel);
        void addToChannel(Channel &channel, Client &client);
        
        int getClient(std::string nick);

        std::string getServerIP(void);
};

std::vector<std::string> create_list(std::string str);

#endif