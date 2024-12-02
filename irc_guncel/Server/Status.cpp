/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgunay <fgunay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:21:44 by fgunay            #+#    #+#             */
/*   Updated: 2024/12/02 17:21:45 by fgunay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server :: checkPollStatus(int poll_status)
{
    if (poll_status == -1) 
    {
        perror("Poll Error!");
        exit(EXIT_FAILURE);
    }
}

void Server :: checkBindStatus(int bind_value)
{
    if (bind_value == -1)
    {
        std::cerr << "Bind Error!" << std::endl;
        exit(1);
    }
}

void Server :: checkListenStatus(int list_value)
{
    if (list_value == -1)
    {
        std::cerr << "listen Error!" << std::endl;
        exit(1);
    }
}

void Server :: checkAcceptStatus(int accept_value)
{
    if (accept_value == -1)
    {
        std::cerr << "Accept Error!" << std::endl;
        exit(1);
    }

    this->acc_value = accept_value;
}

int Server :: checkRecvStatus(int recv_value, int i)
{
    if (recv_value < 0)
    {
        std::cerr << "Recv error!" << std::endl;
        exit(1);
    }
    else if (recv_value == 0)
    {
        this->commands.clear();
        this->commands.push_back("QUIT");
        this->QUIT(this->clients[this->acc_value - 4]);
        std::cout << "Client " << i + 3 << " logged out" << std::endl;
        close(this->fds[i].fd);
        this->fds[i].fd = 0;
        return 0;
    }
    return 1;
}