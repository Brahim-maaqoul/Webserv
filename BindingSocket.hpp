/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BindingSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmaaqoul <bmaaqoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 20:43:40 by bmaaqoul          #+#    #+#             */
/*   Updated: 2023/02/25 18:58:21 by bmaaqoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Socket.hpp"

class BindingSocket : public Socket
{
	private:
		
	public:
		BindingSocket(int domain, int service, int protocol, int port, u_long interface);
		// ~BindingSocket();
		int	connect_to_network(int sock, struct sockaddr_in addr);
};

