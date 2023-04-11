/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmaaqoul <bmaaqoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 17:41:19 by bmaaqoul          #+#    #+#             */
/*   Updated: 2023/04/09 21:00:46 by bmaaqoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ListeningSocket.hpp"

class Server
{
	private:
		ListeningSocket	*socket;
		virtual void	accepter() = 0;
		virtual void	handler() = 0;
		// virtual void	responder() = 0;
	public:
		Server(int domain, int service, int protocol, int port, u_long interface, int backlog);
		// ~Server();
		ListeningSocket	*getSocket();
		virtual void	launch() = 0;
};

