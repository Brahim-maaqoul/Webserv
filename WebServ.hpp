/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmaaqoul <bmaaqoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 18:27:54 by bmaaqoul          #+#    #+#             */
/*   Updated: 2023/04/09 21:31:46 by bmaaqoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include <unistd.h>
#include "Request.hpp"
#include <fstream>

class WebServ : public Server
{
	private:
		char buffer[1024];
		int	new_sockfd;
		
		virtual void	accepter();
		virtual void	handler();
		// virtual void	responder();
	public:
		WebServ();
		// ~WebServ();
		virtual void	launch();
};

