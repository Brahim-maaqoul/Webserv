/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmaaqoul <bmaaqoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 17:41:24 by bmaaqoul          #+#    #+#             */
/*   Updated: 2023/02/25 18:27:12 by bmaaqoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int domain, int service, int protocol, int port, u_long interface, int backlog)
{
	socket = new ListeningSocket(domain, service, protocol, port, interface, backlog);
}

ListeningSocket	*Server::getSocket()
{
	return socket;
}