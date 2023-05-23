/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 13:25:35 by nhanafi            #+#    #+#             */
/*   Updated: 2023/04/17 01:47:32 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "Socket.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include "Location.hpp"

#include "Mimepars.hpp"

class Config
{
	private:
		// Socket socket;
		typedef void (Config::*method_function)(std::string&);
		std::string ip;
		std::string port;
		std::string	root;
		std::string	index;
		std::string server_name;
		long long client_max_body_size;
		std::map <int, std::string> error_page;
		std::vector < std::pair<std::string, Location *> > _location;
	public:
		std::map<std::string, method_function> func;
		Config(std::string &s);
		// std::string gettmp();
		void	map_init(void);
		void	parsServ(std::string &str);
		void	parsLine(std::string &str);
		void	parsBraket(std::string &str);
		void	setip_port(std::string &str);
		void	setroot(std::string &str);
		void	setindex(std::string &str);
		void	setserver_name(std::string &str);
		void	seterror_page(std::string &str);
		void	setclient_size(std::string &str);
		std::string	getport();
		std::string	getip();
		std::string	getname();
		Location	*getLocation(const std::string &str);
		void	geterror(Request &req, Mimepars &types);
		unsigned long long getclient_max_body_size();
		// void	getroutes(std::string &str);
		~Config();
};


