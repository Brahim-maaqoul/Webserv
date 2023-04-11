/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectingSocket.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmaaqoul <bmaaqoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 11:34:14 by bmaaqoul          #+#    #+#             */
/*   Updated: 2023/02/25 18:58:18 by bmaaqoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Socket.hpp"

class ConnectingSocket : public Socket
{
	private:
		/* data */
	public:
		ConnectingSocket(int domain, int service, int protocol, int port, u_long interface);
		// ~ConnectingSocket();
		virtual	int connect_to_network(int sock, struct sockaddr_in address);
};

