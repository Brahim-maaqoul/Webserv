/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmaaqoul <bmaaqoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 11:46:56 by bmaaqoul          #+#    #+#             */
/*   Updated: 2023/02/25 12:10:43 by bmaaqoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(int domain, int service, int protocol,
	int port, u_long interface, int backlog) : BindingSocket(domain, service, protocol, port, interface)
{
	backlog_ = backlog;
	start_listening();
	test_connection(listening_);
}

void	ListeningSocket::start_listening()
{
	listening_ = listen(getSock(), backlog_);
}

int	ListeningSocket::getBacklog()
{
	return backlog_;
}

int	ListeningSocket::getListening()
{
	return listening_;
}