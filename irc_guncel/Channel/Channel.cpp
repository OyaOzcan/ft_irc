/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgunay <fgunay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:19:18 by fgunay            #+#    #+#             */
/*   Updated: 2024/12/02 17:19:19 by fgunay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel()
{
    this->topic = "";
    this->name = "";
    this->key = "";
    this->required_key = false;
    this->invite_only = false;
}

Channel::Channel(std::string name)
{
    this->topic = "";
    this->name = name;
    this->key = "";
    this->required_key = false;
    this->invite_only = false;
}
Channel::Channel(std::string name, std::string key)
{
    this->topic = "";
    this->name = name;
    this->key = key;
    this->required_key = true;
    this->invite_only = false;
}


Channel::Channel(Channel const &copy){*this = copy;}

void    Channel::operator=(Channel const &copy)
{
    this->topic = copy.topic;
    this->name = copy.name;
    this->Operators = copy.Operators;
    this->members = copy.members;
    this->key = copy.key;
    this->required_key = copy.required_key;
    this->invite_only = copy.invite_only;
}

Channel::~Channel(){}

std::string Channel::getName(){return this->name;}

std::vector<Client> &Channel::getMembers(){return this->members;}
std::vector<Client> &Channel::getOperators(){return this->Operators;}

bool Channel::getRequiredKey(){return this->required_key;}
bool Channel::getInviteOnly(){return this->invite_only;}

std::string &Channel::getKey(void){return this->key;}
std::string &Channel::getTopic(void){return this->topic;}

void Channel::addMembers(Client const &client){this->members.push_back(client);}
void Channel::addOperators(Client const &client){ this->Operators.push_back(client);}

bool Channel :: isMember(Client const &client)
{
    for (size_t i = 0; i < this->members.size(); i++)
    {
        if (this->members[i].getNick() == client.getNick())
            return true;
    }
    return false;
}

bool Channel :: isOperator(Client const &client)
{
    for (size_t i = 0; i < this->Operators.size(); i++)
    {
        if (this->Operators[i].getNick() == client.getNick())
            return true;
    }
    return false;
}


void Channel :: printMembers()
{
    for (size_t i = 0; i < this->members.size(); i++)
    {
        std::cout << this->members[i].getNick() << std::endl;
    }
}

void Channel :: setTopic(std::string topic){this->topic = topic;}


