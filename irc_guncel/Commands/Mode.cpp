/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgunay <fgunay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:19:06 by fgunay            #+#    #+#             */
/*   Updated: 2024/12/02 17:21:12 by fgunay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server/Server.hpp"

void Server :: MODE(Client &client)
{
    if (this->checkActivation(client) == -1)
        return;
    
    if (this->commands.size() != 3 && this->commands.size() != 4)
    {
        client.print(client.getNick() + " MODE :Not enough parameters\n");
        return;
    }
    
    if (this->commands[1][0] != '#')
    {
        client.print("Try #channel_name\r\n");
        return;
    }
    if (!this->isChannelExist(this->commands[1]))
    {
        client.print(client.getNick() + " :No such channel\n");
        return;
    }
    else
    {
        Channel &channel = this->channels[getChannelIndex(this->commands[1])];
        if (!channel.isOperator(client))
        {
            client.print(client.getNick() +  " " + channel.getName() + " :You're not channel operator\n");
            return ;
        }
        
        if (this->commands[2][1] == 'i' && this->commands[2].size() == 2)
            channel.inviteOnly(this->commands[2][0], client);
        else if (this->commands[2][1] == 'o' && this->commands[2].size() == 2)
        {
            if (this->getClient(this->commands[3]) == -1)
            {
                client.print(client.getUsername() + ": " + this->commands[3] + " :No such nick/channel\r\n");
                return ;
            }
            channel.operators(this->commands[2][0], this->clients[this->getClient(this->commands[3])], client);
        }
        else if (this->commands[2][1] == 'k' && this->commands[2].size() == 2)
            channel.keys(this->commands[2][0], this->commands[3], client);
        else
            client.print(client.getNick() + " :Unknown MODE flag\n");
    }

}
