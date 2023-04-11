/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmaaqoul <bmaaqoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 17:55:00 by bmaaqoul          #+#    #+#             */
/*   Updated: 2023/02/25 18:58:06 by bmaaqoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

class Socket
{
	private:
		struct sockaddr_in address;
		int sock_fd;
	public:
		Socket(int domain, int service, int protocol, int port, u_long interface);
		// virtual ~Socket();
		virtual int	connect_to_network(int sock, struct sockaddr_in addr) = 0;
		void	test_connection(int item);
		struct sockaddr_in getAddress();
		int	getSock();
		void	setConnection(int connect);
};

