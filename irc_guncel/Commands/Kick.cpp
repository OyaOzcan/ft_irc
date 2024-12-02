/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgunay <fgunay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:21:05 by fgunay            #+#    #+#             */
/*   Updated: 2024/12/02 18:14:32 by fgunay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server/Server.hpp"

void Server :: KICK(Client &client)
{
    size_t m = 0;
    if(this->checkActivation(client) == -1)
        return ;
    std::string comment = "";
    for (size_t j = 0; j < this->commands.size(); j++)
    {
        comment += this->commands[j];
        comment += " ";
    }
    Channel & channel = this->channels[getChannelIndex(this->commands[1])];
    if(this->commands.size() < 3)
    {
        client.print(":" + this->getServerIP() + " 461 " + client.getNick() + "KICK : Not enough parameters\r\n");
        return ;
    }
    for (; m < channel.getOperators().size(); m++)
    {
        if(client.getNick().compare(channel.getOperators()[m].getNick()) == 0)
        break ;
    }
    if (channel.getOperators().empty() || m == channel.getOperators().size())
    {
        client.print(":" + this->getServerIP() + " 482 " + client.getNick() + "KICK :You're not channel operator\r\n");
        return ;
    }
    else if(this->isChannelExist(this->commands[1]) == false)
    {
        client.print(":" + this->getServerIP() + " 403 " + client.getNick() + "KICK: No such channel\r\n");
        return ;
    }
    else if(this->isChannelExist(this->commands[1]) == true && channel.isMember(client) == false)
    {
        client.print(":" + this->getServerIP() + " 442 " + client.getNick() + "KICK: You are not on that channel\r\n");
        return;
    }
    else if(this->isChannelExist(this->commands[1]) == true && channel.isMember(channel
    .getMembers()[getClientIndex(this->commands[2],  channel)]) == false)
    {
        client.print(":" + this->getServerIP() + " 441 " + client.getNick() + "KICK: They aren't on that channel\r\n");
        return ;
    }
    else
    {
        size_t targetIndex = getClientIndex(this->commands[2], channel);
if (targetIndex < channel.getMembers().size())
{
    for (size_t m = 0; m < channel.getMembers().size(); m++)
        channel.getMembers()[m].print("KICK " + channel.getName() + " " + client.getNick() + comment);
    channel.getMembers().erase(channel.getMembers().begin() + targetIndex);
}

    }
}