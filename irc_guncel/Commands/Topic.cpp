/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgunay <fgunay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:21:30 by fgunay            #+#    #+#             */
/*   Updated: 2024/12/02 17:21:31 by fgunay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server/Server.hpp"

void Server::TOPIC(Client &client)
{
    if(this->checkActivation(client) == -1)
        return ;
    std::string topic = "";

    if (this->commands.size() < 2)
    {
        client.print(":" + this->getServerIP() + " 461 "+ client.getNick() + " TOPIC :Not enough parameters" + "\r\n");
        return ;
    }
    if (this->commands.size() == 3)
        topic = this->commands[1].substr(1);
    if (this->isChannelExist(this->commands[1]))
    {
        Channel & channel = this->channels[getChannelIndex(this->commands[1])];
        for (size_t j = 3; j < this->commands.size(); j++)
            topic += " " + this->commands[j];
        if (this->commands[1][0] == ':')
        {
            if (!topic.compare(":"))
                channel.setTopic("");
            else
                channel.setTopic(topic);
        }
        else
            client.print(":" + this->getServerIP() + " 332 " + client.getNick() + " "+ channel.getName() + ":" + channel.getTopic() + "\r\n");
    }
    else
    {
        client.print(":" + this->getServerIP() + " 403 " + client.getNick() + " " + this->commands[1] + " :No such channel\r\n");
        return ;
    }
}