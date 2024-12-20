/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgunay <fgunay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:21:23 by fgunay            #+#    #+#             */
/*   Updated: 2024/12/02 17:58:22 by fgunay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server/Server.hpp"

void Server :: PASS(Client &client)
{
    if (client.getLoggedStatus() == true || client.getisConnected() == true)
    {
        client.print(":" + this->getServerIP() + " 462 " + client.getNick() + " :You may not register\r\n");
        return ;
    }

    if (this->commands.size() < 2)
    {
        client.print(":" + this->getServerIP() + " 461 " + client.getNick() + "PASS : Not enough parameters\r\n");
        return ;
    }

    if (this->password == this->commands[1].substr(1))
    {
        client.print("You are in the system\n");
        client.setisConnected(true);

        std::vector<std::string> coms(this->commands);
        if (this->commands.size() >= 4 && this->commands[2] == "NICK")
        {
            this->commands.clear();
            this->commands.push_back(coms[2]);
            this->commands.push_back(coms[3]);
            this->NICK(client);
            if (coms.size() == 9 && coms[4] == "USER")
            {
                this->commands.clear();
                this->commands.push_back(coms[4]);
                this->commands.push_back(coms[5]);
                this->commands.push_back(coms[6]);
                this->commands.push_back(coms[7]);
                this->commands.push_back(coms[8]);
                this->USER(client);
            }
        }
    }
    else
    {
        client.print(":" + this->getServerIP() + " 464 " + client.getNick() + "PASS : Password incorrect\r\n");
    }
}
