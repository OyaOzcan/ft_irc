/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgunay <fgunay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:21:26 by fgunay            #+#    #+#             */
/*   Updated: 2024/12/02 19:09:03 by fgunay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server/Server.hpp"

void Server::PRIVMSG(Client &client)
{
    size_t i = 0;
    if (this->checkActivation(client) == -1)
        return;

    std::string message = "";
    if (this->commands.size() > 2)
        message += this->commands[2].substr(0); // SUBSTR İŞLEVİ KONTROL EDİLECEK
    for (size_t j = 3; j < this->commands.size(); j++)
    {
        message += " ";
        message += this->commands[j];
    }

    if (this->commands.size() == 1)
    {
        client.print(":" + this->getServerIP() + " 411 " + client.getNick() + ": No recipient given PRIVMSG\r\n");
        return;
    }
    else if (this->commands.size() == 2)
    {
        client.print(":" + this->getServerIP() + " 412 " + client.getNick() + ": No text to send PRIVMSG\r\n");
        return;
    }

    for (; i < this->channels.size(); i++)
    {
        if (this->commands[1].compare(this->channels[i].getName()) == 0)
        {
            // Kanal üyeliği kontrolü
            if (!this->channels[i].isMember(client))
            {
                client.print(":" + this->getServerIP() + " 404 " + client.getNick() + " " + this->channels[i].getName() + " :Cannot send to channel\r\n");
                return;
            }

            // Mesajı kanal üyelerine gönder
            for (size_t m = 0; m < this->channels[i].getMembers().size(); m++)
            {
                if (this->channels[i].getMembers()[m].getNick() != client.getNick())
                {
                    this->channels[i].getMembers()[m].print(":" + client.getNick() + "!" + client.getUsername() + '@' + client.getRealIp() + " PRIVMSG " + this->channels[i].getName() + " : " + message + "\r\n");
                }
            }
            return;
        }
    }

    // Kullanıcıya veya kanala mesaj gönderilemiyorsa
    for (i = 0; i < this->clients.size(); i++)
    {
        if (this->commands[1].compare(this->clients[i].getNick()) == 0)
        {
            clients[i].print(":" + client.getNick() + "!" + client.getUsername() + '@' + client.getRealIp() + " PRIVMSG " + clients[i].getNick() + " :" + message + "\r\n");
            client.print(":" + client.getNick() + "!" + client.getUsername() + '@' + client.getRealIp() + " PRIVMSG " + clients[i].getNick() + " :" + message + "\r\n");
            return;
        }
    }

    client.print(":" + this->getServerIP() + " 401 " + client.getNick() + ": No such nick/channel\r\n");
    return;
}
