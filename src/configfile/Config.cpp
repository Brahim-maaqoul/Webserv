/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmaaqoul <bmaaqoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 18:48:20 by nhanafi           #+#    #+#             */
/*   Updated: 2023/05/23 18:35:58 by bmaaqoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

bool sortfuc(std::pair<std::string, Location *> &a, std::pair<std::string, Location *> &b)
{
	return a.first.size() > b.first.size();
}

Config::Config(std::string &str):ip("127.0.0.1"),port("80"),client_max_body_size(-1)
{
	if (str.empty() || str.find_first_not_of("\n\t ") == std::string::npos)
		throw "server empty" + str;
	map_init();
	parsServ(str);
	std::sort(_location.begin(), _location.end(), sortfuc);
}

std::string Config::getport()
{
	return port;
}

std::string Config::getip()
{
	return ip;
}

std::string Config::getname()
{
	return server_name;
}


void	Config::setip_port(std::string &str)
{
	if (str.empty())
		throw "empty arrgument";
	if (str.find_first_of(":") != str.npos)
	{
		if (str.find_first_of(":") != str.find_last_of(":"))
			throw "unexpexted ':' " + str;
		
		ip = str.substr(0,str.find(':'));
		port = str.substr(str.find(":") + 1);
		if (std::count(ip.begin(), ip.end(), '.') != 3 || ip.find_first_not_of("0123456789.") != ip.npos)
			throw "invalid ip: " + ip;
		if (port.find_first_not_of("0123456789") != ip.npos)
			throw "invalid port: " + ip;
		return;
	}
	if (str.find(',') != str.npos)
	{
		ip = str;
		if (std::count(ip.begin(), ip.end(), '.') != 3 || ip.find_first_not_of("0123456789.") != ip.npos)
			throw "invalid ip: " + ip;
		return ;
	}
	port = str;
	if (port.find_first_not_of("0123456789") != port.npos)
		throw "invalid port: " + port;
}

void	Config::setroot(std::string &str)
{
	if (!root.empty())
		throw "multiple roots" + str;
	Location *tmp = new Location;
	tmp->setlocation("/");
	tmp->setroot(str);
	_location.push_back(std::make_pair("/", tmp));
	root = str;
}

void	Config::setindex(std::string &str)
{
	if (!index.empty())
		throw "multiple index" + str;
	index = str;
}

void	Config::setserver_name(std::string &str)
{
	if (!server_name.empty())
		throw "multiple server name" + str;
	server_name = str;
}

void	Config::setclient_size(std::string &str)
{
	trim(str, "\n\t; ");
	if (str.size() > 17 ||str.find_first_not_of("0123456789kmKMgG") != str.npos ||\
		str.find_first_of("kmgKMG") != str.find_last_of("kmgKMG")||
		(str.find_first_of("kmgKMG") != str.npos && str.find_first_of("kmgKMG") != (str.size() - 1)))
	 	throw "invalid number in body size syntax " + str;
	
	cast(str ,client_max_body_size);
	if (str[str.size() - 1] == 'k' || str[str.size() - 1] == 'K' )
	{
		if(client_max_body_size > 1e9)
	 		throw "invalid number in body size syntax " + str;
		client_max_body_size *= 1024;
	}
	else if (str[str.size() - 1] == 'm' || str[str.size() - 1] == 'M' )
	{
		if(client_max_body_size > 1e6)
			throw "invalid number in body size syntax " + str;
		client_max_body_size *= 1024 * 1024;
	}
	else if (str[str.size() - 1] == 'g' || str[str.size() - 1] == 'G')
	{
		if(client_max_body_size > 1000)
			throw "invalid number in body size syntax " + str;
		client_max_body_size *= 1024 * 1024 * 1024;
	}
}

void	Config::seterror_page(std::string &str)
{
	if (str.find(' ') == str.npos)
		throw "invalid error page " + str;
	std::string page = str.substr(str.find_last_of(' ') + 1);
	str = str.substr(0, str.find_last_of(' '));
	trim(str, " \t\n");
	if (str.empty())
		throw "invalid error type ";
	while(!str.empty())
	{
		std::string tmp;
		if (str.find_first_of(" \t") == str.npos)
		{
			tmp = str;
			str = "";	
		}
		else
		{
			tmp = str.substr(0, str.find_first_of(" \t"));
			str = str.substr(str.find_first_of(" \t"));	
		}
		if (tmp.find_first_not_of("0123456789") != tmp.npos)
			throw "invalid error number " + tmp;
		error_page[atoi(tmp.c_str())] = page;
		trim(str, " \t\n");
	}
}


void	Config::map_init(void)
{
	this->func["listen"] = &Config::setip_port;
	this->func["root"] = &Config::setroot;
	this->func["index"] = &Config::setindex;
	this->func["error_page"] = &Config::seterror_page;
	this->func["server_name"] = &Config::setserver_name;
	this->func["client_max_body_size"] = &Config::setclient_size;
}


void	Config::parsServ(std::string &str)
{
	if (str.empty() || str.find_first_not_of("\n\t ") == std::string::npos)
		return ;
	str = str.substr(str.find_first_not_of("\n\t "));
	if (str[0] == '#')
	{
		if (str.find_first_of("\n") == std::string::npos)
			return ;
		str = str.substr(str.find_first_of("\n"));
		return parsServ(str);
	}
	if (str.find_first_of(";{") == std::string::npos)
		throw "expected ';'" + str;
	std::string tmp = str.substr(0, str.find_first_of(";{") + 1);
	str = str.substr(str.find_first_of(";{") + 1);
	if (tmp[tmp.size() - 1] == ';')
		parsLine(tmp);
	else
	{
		if (str.find_first_of("}") == std::string::npos)
			throw "error braket" + str;
		tmp += str.substr(0, str.find_first_of("}") + 1);
		str = str.substr(str.find_first_of("}") + 1);
		parsBraket(tmp);
	}
	parsServ(str);
}

void	Config::parsLine(std::string &str)
{
	std::string	key;

	trim(str, "\n\t ;");
	if (str.find('#') != str.npos)
		throw "unexpected comment #" + str;
	if (str.find_first_of(" \t\n") == std::string::npos)
		throw "invalid argument" + str;
	key = str.substr(0 ,str.find_first_of(" \t\n"));
	str = (str.substr(str.find_first_of(" \t\n")));
	trim(str, "\n\t ;");
	if (str.empty())
		throw "invalid argument";
	if (this->func.find(key) == this->func.end())
	{
		throw "invalid argument" + str;
	}
	(this->*func[key])(str);
	
}

void	Config::parsBraket(std::string &str)
{
	trim(str, " \t\n");
	if (str.find_first_of(" t") == str.npos)
		throw "error location" + str;
	std::string tmp = str.substr(0, str.find_first_of(" \t"));
	if (tmp != "location")
		throw "unknown variable" + tmp;
	str = str.substr(str.find_first_of(" \t"));
	Location *L = new Location(str);
	_location.push_back(std::make_pair(L->getlocation(), L));
}


Location	*Config::getLocation(const std::string &str)
{
	for (size_t i = 0; i < _location.size(); i++)
	{
		if (str.find(_location[i].first) == 0 \
		&& (str[_location[i].first.size()] == 0\
		|| str[_location[i].first.size()] == '/'
		|| _location[i].first[_location[i].first.size() - 1] == '/'))
			return _location[i].second;
	}
	return NULL;
}


void	Config::geterror(Request &req, Mimepars &types)
{
	std::string url = "";
	if (error_page.find(req.getStatus()) != error_page.end())
	{
		url = getLocation("/")->getroot() + "/" + error_page[req.getStatus()];
	}
	if (url.empty() || isdir(url))
	{
		url = "./www/" + std::to_string(req.getStatus()) + ".html";
		if (isdir(url))
			return req.reshandler();
	}
    req.setcontent_type(types.gettype(url.substr(url.find_last_of("."))));
	req.setUrl(url);
    req.response.url = url;
	req.response.body_exist = 1;
	req.response.total_range = std::make_pair(-1, -1);
    req.reshandler();
}

unsigned long long Config::getclient_max_body_size()
{
	return client_max_body_size;
}

Config::~Config()
{
	for (size_t i = 0; i < _location.size(); i++)
	{
		delete _location[i].second;
	}
}