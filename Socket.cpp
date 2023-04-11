/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmaaqoul <bmaaqoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 17:56:11 by bmaaqoul          #+#    #+#             */
/*   Updated: 2023/02/25 12:13:06 by bmaaqoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(int domain, int service, int protocol, int port, u_long interface)
{
	address.sin_family = domain;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = htonl(interface);
	sock_fd = socket(domain, service, protocol);
	test_connection(sock_fd);
}

void	Socket::test_connection(int item)
{
	if (item < 0)
	{
		perror("Failed to connect...");
		exit (EXIT_FAILURE);
	}
}

struct	sockaddr_in	Socket::getAddress()
{
	return	address;
}

int	Socket::getSock()
{
	return sock_fd;
}
