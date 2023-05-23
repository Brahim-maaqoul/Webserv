/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 10:39:33 by nhanafi           #+#    #+#             */
/*   Updated: 2023/05/21 00:56:56 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h> // socket() listen() bind() getaddrinfo() // gai_strerror() freeaddrinfo()
# include <sys/types.h> // getaddrinfo() gai_strerror() freeaddrinfo()
# include <netdb.h> // getaddrinfo() gai_strerror() freeaddrinfo()
# include <netinet/in.h>
# include <stdio.h> // strerror
# include <sys/errno.h>
# include <iostream>
# include <unistd.h> // close
# include <arpa/inet.h> // inet_ntop
# include <poll.h>

# define BACKLOG 100000

# define PORT "9090"

class Socket
{
	public:
		std::string _port;
		int	_socket_fd;
		int	_connection;

		//bind	
		struct addrinfo	_hints;
		struct addrinfo	*_servinfo;

		//poll
		struct pollfd *mypoll;

		int				_status;
		int				_pipe;
	public:
		Socket(std::string port);
		Socket create_socket();
		// Socket(std::string port);
		// void	create();
		// void	_bind();
		// void	_listen();
		// void	_accept();
		// void	_send();
		// int		get_socket_fd() {return _socket_fd;}
		~Socket();
};

int guard(int n, char * err);
#endif