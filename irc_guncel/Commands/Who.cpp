/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgunay <fgunay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:21:35 by fgunay            #+#    #+#             */
/*   Updated: 2024/12/02 17:21:36 by fgunay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server/Server.hpp"

void Server :: WHO(Client &client)
{
    size_t i = 0;
    if(this->checkActivation(client) == -1)
        return ;
    if(this->commands[1][0] == '#' && this->isChannelExist(this->commands[1]))
    {
        for (size_t j = 0; j < this->channels[getChannelIndex(this->commands[1])].getMembers().size(); j++)
        {
            client.print("Nick: " + this->clients[j].getNick() + "\n");
        }
        client.print(":" + client.getRealIp() + " 315 " +  this->commands[1] + ": End of WHO list\r\n");
        return ;
    }
    else
    {
        for (; i < this->channels.size(); i++)
        {
            for (size_t m = 0; m < this->channels[i].getMembers().size(); m++)
            {
                if(this->commands[1].compare(this->channels[i].getMembers()[m].getNick()) == 0)
                {
                    client.print("Nick: " + this->channels[i].getMembers()[m].getNick() + "\n");
                    client.print("Ip: " + this->channels[i].getMembers()[m].getRealIp() + "\n");
                    client.print("realName: " + this->channels[i].getMembers()[m].getRealName() + "\n");
                    client.print("Username: " + this->channels[i].getMembers()[m].getUsername() + "\n");
                    break ;
                }
            }
        }
    }
    client.print(":" + client.getRealIp() + " 315 " + client.getNick() + ": End of WHO list\r\n");
}
