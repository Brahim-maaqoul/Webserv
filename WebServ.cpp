/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmaaqoul <bmaaqoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 18:28:01 by bmaaqoul          #+#    #+#             */
/*   Updated: 2023/04/10 18:39:06 by bmaaqoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <cstring>

WebServ::WebServ() : Server(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
	launch();
}

void	WebServ::accepter()
{
	struct sockaddr_in address = getSocket()->getAddress();
	int	addrlen = sizeof(address);
	new_sockfd = accept(getSocket()->getSock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
	read(new_sockfd, buffer, 1024);
}

void	WebServ::handler()
{
	Request	r;
	std::cout << buffer << std::endl;
	std::string	buff(buffer);
	r.parse_request(buff);
	if (r.getUrl().find(".mp3") != std::string::npos)
	{
		std::ifstream file("/Users/bmaaqoul/Desktop/webserv-1/two.mp3");
		std::stringstream ss;
		ss << file.rdbuf();
		std::string s;
		// while(getline(file, s))
		// {
		// 	response.append(s);
		// }
		s.append(ss.str());
		std::string response = "HTTP/1.1 200 OK\r\nContent-Type: audio/mpeg\r\nContent-Length: ";
		response += (std::to_string(s.size()));
		response += ("\r\n\r\n");
		std::cout << response << std::endl;
		response += (s);
		// std::string hello = "\">\n</body>\n</html>\n";
		// response.append(hello);
		write(new_sockfd, response.c_str(), response.length());
		close(new_sockfd);
		std::cout << "===============img=================" << std::endl << std::endl;
		return;
	}
	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\r\n\r\n<html>\n<body>\n<audio><source src=\"/two.mp3";
	// std::ifstream file(r.getUrl());
	// std::string s;
	// while(getline(file, s))
	// {
	// 	response.append(s);
	// }
	// response.append(r.getUrl());
	std::string hello = "\">></audio>\n</body>\n</html>\n";
	response.append(hello);
	write(new_sockfd, response.c_str(), response.length());
	close(new_sockfd);
}

// void	WebServ::responder()
// {
// }

void	WebServ::launch()
{
	while (1)
	{
		std::cout << "======= WAITING ========\n";
		accepter();
		handler();
		// responder();
		std::cout << "======== DONE ========\n";
	}
}