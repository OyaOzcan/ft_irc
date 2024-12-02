/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgunay <fgunay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:21:02 by fgunay            #+#    #+#             */
/*   Updated: 2024/12/02 17:21:03 by fgunay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server/Server.hpp"

void Server :: JOIN(Client &client)
{
    std::string channels = this->commands[1];
    std::vector<std::string> key_list;
    std::string keys;

    if(this->checkActivation(client) == -1)
        return ;

    if (this->commands.size() < 2)
    {
        client.print(":" + client.getRealIp() + " 461 " + client.getNick() + " JOIN : Not enough parameters\r\n");
        return ;
    }
    if (this->commands.size() < 3)
    {
        keys = "";
        key_list.push_back("");
    }
    else
    {
        keys = this->commands[2];
        key_list = create_list(keys);
    }
    if (channels == "0" && this->commands.size() == 2)
    {
        for (size_t m = 0; m < this->channels.size(); m++)
        {
            this->commands.clear();
            this->commands.push_back("PART");
            this->commands.push_back(this->channels[m].getName());
            this->PART(client);
        }
        return ;
    }
    std::vector<std::string> channel_list;
    channel_list = create_list(channels);

    for (size_t i = 0; i < channel_list.size(); i++)
    {
        if(channel_list[i][0] != '#')
        {
            client.print("JOIN: There is no # in the first character.\n");
            channel_list.clear();
            key_list.clear();
            return ;
        }
        if (this->isChannelExist(channel_list[i]))
        {
            Channel & channel = this->channels[getChannelIndex(channel_list[i])];
            if (channel.getRequiredKey() && !channel.getInviteOnly())
            {
                if (!keys.empty())
                {
                    if (key_list[i] == channel.getKey())
                        this->addToChannel(channel, client);
                }
                else
                {
                    client.print(":" + this->getServerIP() + " 475 " + client.getNick() + " " + channel.getName() + " :Cannot join channel (+k)" + "\r\n");
                    channel_list.clear();
                    key_list.clear();
                    return ;
                }
            }
            else
            {
                if (channel.getInviteOnly() && !client.isChannelInvited(channel.getName()))
                {
                    client.print(":" + this->getServerIP() + " 473 " + client.getNick() + " " + channel.getName() + " :Cannot join channel (+i)\n");
                    channel_list.clear();
                    key_list.clear();
                    return ;
                }
                else
                    this->addToChannel(channel, client);
            }
        }
        else
        {
            if (key_list[i] != "")
            {
                Channel channel(channel_list[i], key_list[i]);
                this->channels.push_back(channel);

                std::cout << client.getNick() << " created channel: " << channel_list[i] << " that requires key" << std::endl;
            }
            else
            {
                Channel channel(channel_list[i]);
                this->channels.push_back(channel);

                std::cout << client.getNick() << " created channel: " << channel_list[i] << " that doesn't require key" << std::endl;
            }
            Channel &channel1 = this->channels[getChannelIndex(channel_list[i])];
            channel1.addOperators(client);
            this->addToChannel(channel1, client);
        }
    }
    channel_list.clear();
    key_list.clear();
}

