/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgunay <fgunay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:21:47 by fgunay            #+#    #+#             */
/*   Updated: 2024/12/02 17:21:48 by fgunay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::vector<std::string> create_list(std::string str)
{
    std::vector<std::string> new_list;

    size_t end = str.find(',');
    size_t start = 0;

    while (end != std::string::npos)
    {
        new_list.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(',', start);
    }
    if (start != str.length())
        new_list.push_back(str.substr(start));
    else
        new_list.push_back("");
    std::cout << new_list.back() << std::endl;
    return(new_list);
}

int Server :: getClient(std::string nick)
{
    for (size_t i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i].getNick() == nick)
            return i;
    }

    return -1;
}

int Channel :: getOperatorIndex(std::string nick)
{
    for (size_t i = 0; i < this->Operators.size(); i++)
    {
        if (this->Operators[i].getNick() == nick)
            return i;
    }
    return -1;
}


void Channel :: inviteOnly(char command, Client &client)
{
    if (command == '-')
    {
        this->invite_only = false;
        client.print("Channel is not invite only\n");
    }
    else if (command == '+')
    {
        this->invite_only = true;
        client.print("Channel is invite only\n");
    }
    else
        client.print(client.getNick() + " :Unknown MODE flag\n");
}

void Channel :: operators(char command, Client &operators, Client &client)
{
    if (command == '-')
    {
        if (this->isOperator(operators))
        {
            this->Operators.erase(this->Operators.begin() + this->getOperatorIndex(operators.getNick()));
            client.print(operators.getNick() + " is deleted from operators list of the " + this->getName() + " channel\n");
        }   
    }
    else if (command == '+')
    {
        this->Operators.push_back(operators);
        client.print(operators.getNick() + " is added as an operator in " + this->getName() + " channel\n");
    }
    else
        client.print(client.getNick() + " :Unknown MODE flag\n");
}

void Channel :: keys(char command, std::string key, Client &client)
{
    if (command == '-')
    {
        this->required_key = false;
        this->key = "";
        client.print("Channel doesn't requere a key\n");
    }
    else if (command == '+')
    {
        this->required_key = true;
        this->key = key;
        client.print("Channel requeres a key\n");
    }
    else
        client.print(client.getNick() + " :Unknown MODE flag\n");
}

