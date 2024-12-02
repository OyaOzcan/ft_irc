/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgunay <fgunay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:21:49 by fgunay            #+#    #+#             */
/*   Updated: 2024/12/02 17:21:50 by fgunay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Server.hpp"

int main(int ac, char **av)
{
    if(ac != 3)
    {
        std::cerr << "Usage: ./irserv <port> <password>" << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        Server serv(atoi(av[1]), av[2]);
        serv.runServer();
    }
    return 0;
}