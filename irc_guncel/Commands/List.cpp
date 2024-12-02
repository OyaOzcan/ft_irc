/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgunay <fgunay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:21:07 by fgunay            #+#    #+#             */
/*   Updated: 2024/12/02 17:21:08 by fgunay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server/Server.hpp"

void Server :: LIST(Client &client)
{
    if(this->checkActivation(client) == -1)
        return ;
    std::string list;
    for(size_t i = 0; i < this->channels.size(); i++)
    {
        list += this->channels[i].getName() + " " + this->channels[i].getTopic() + " ";
    }
    client.print(":" + client.getRealIp() + " " + list + "\r\n");
}