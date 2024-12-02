/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgunay <fgunay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:21:28 by fgunay            #+#    #+#             */
/*   Updated: 2024/12/02 17:21:29 by fgunay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server/Server.hpp"

void Server::QUIT(Client &client)
{
    std::string reason;
    if(this->commands[0].size() == 5)
    {
        std::cout << "QUIT: Client exiting from the network\r\n";
        close(this->acc_value);
    }
    else
    {
        for (size_t i = 0; i < this->commands.size(); i++)
            reason += this->commands[i];
        std::cout << "QUIT:" << reason << std::endl;
        
        for (size_t m = 0; m < this->channels.size(); m++)
        {
            this->commands.clear();
            this->commands.push_back("PART");
            this->commands.push_back(this->channels[m].getName());
            this->PART(client);
        }
        for (size_t n = 0; n < this->clients.size(); n++)
        {
            if (this->clients[n].getNick() == client.getNick())
            {
                this->clients.erase(this->clients.begin() + n);
                break ;
            }
        }
        client.setLoggin(false);
        this->commands.clear();
        close(this->acc_value);
    }
    close(this->acc_value);

}