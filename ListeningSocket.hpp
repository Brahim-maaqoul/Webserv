/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmaaqoul <bmaaqoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 11:46:59 by bmaaqoul          #+#    #+#             */
/*   Updated: 2023/02/25 18:58:13 by bmaaqoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "BindingSocket.hpp"

class ListeningSocket : public BindingSocket
{
	private:
		int	backlog_;
		int	listening_;
	public:
		ListeningSocket(int domain, int service, int protocol,
			int port, u_long interface, int backlog);
		// ~ListeningSocket();
		void	start_listening();
		int	getBacklog();
		int	getListening();
};

