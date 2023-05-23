
#include "Socket.hpp"

#include <fstream>
#include <string>
#include <fcntl.h>
#include <sstream>

Socket::Socket(std::string port) : _port(port)
{

}

Socket Socket::create_socket()
{
	int yes = 1;

	bzero(&_hints, sizeof(_hints));
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;
	if ((_status = getaddrinfo(NULL, _port.c_str(), &_hints, &_servinfo)) != 0)
	{
    	std::cout << "getaddrinfo error: " << gai_strerror(_status) << std::endl;
		exit(1);
	}
	struct addrinfo *p;
	for (p = _servinfo; p != NULL; p = p->ai_next)
	{
		if ((_socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			std::cerr << "server: socket error\n";
			exit(1);
		}
		if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) == -1)
		{
			std::cout << "listen error: " << strerror(errno) << std::endl;
			exit(1);
		}
		if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR , &yes, sizeof(yes)) == -1)
		{
			std::cerr << "setsockopt error\n";
			exit(1);
		}
		if (setsockopt(_socket_fd, SOL_SOCKET, SO_NOSIGPIPE , &yes, sizeof(yes)) == -1)
		{
			std::cerr << "setsockopt error\n";
			exit(1);
		}
		if (setsockopt(_socket_fd, SOL_SOCKET, SO_KEEPALIVE , &yes, sizeof(yes)) == -1)
		{
			std::cerr << "setsockopt error\n";
			exit(1);
		}
		if (bind(_socket_fd, p->ai_addr, p->ai_addrlen) == -1)
		{
			std::cerr << "bind error\n";
			close(_socket_fd);
			exit(1);
		}
	}
	if (listen(_socket_fd, BACKLOG) != 0)
	{
		std::cout << "listen error: " << strerror(errno) << std::endl;
		close(_socket_fd);
	}
	return (*this);
}

Socket::~Socket()
{

}
