/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 09:15:58 by nhanafi           #+#    #+#             */
/*   Updated: 2023/05/21 01:15:06 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"



Location::Location():matching(0),root(""),autoindex(1),method(1),index(""),cgi(0)
{
}

Location::Location(std::string &str):matching(0),root(""),autoindex(1),method(1),index(""),cgi(0)
{
	mapinit();
	trim(str, " \t\n;");
	if (str[0] == '=')
	{
		matching = 1;
		str = str.substr(1);
		trim(str, " \t\n");
	}
	if (str.find('{') == str.npos || str.find('}') == str.npos)
		throw "braket error" + str;
	location = str.substr(0, str.find('{'));
	trim(location, " \t\n");
	str = str.substr(str.find('{'), str.find('}'));
	trim(str, " \t\n}{");
	pars(str);
}


void Location::mapinit()
{
	func["root"] = &Location::setroot;
	func["autoindex"] = &Location::setautoindex;
	func["methods"] = &Location::setmethod;
	func["index"] = &Location::setindex;
	func["cgi"] = &Location::setcgi;
}

void Location::setindex(std::string &str)
{
	trim(str, "\n\t ;");
	if (str.empty() || str.find_first_of(" \t\n") != str.npos)
		throw "error index" + str;
	index = str;
}

void Location::setcgi(std::string &str)
{
	cgi = 1;
	trim(str, "\n\t; ");
	if (str.empty())
		throw "error cgi argument" + str;
	std::stringstream ss(str);
	std::string key, val1,val2;
	if (!(ss >> key >> val1))
		throw "error cgi argument" + str;
	if (ss >> val2)
	{
		cgi_cmd[key] = std::make_pair(val1, val2);
	}
	else
	{
		cgi_cmd[key] = std::make_pair(val1, "");
	}
	if (ss >> val2 || (key != ".py" && key != ".pl" && key != ".rb"))
		throw "undifined cgi: " + key;
	if (access(val1.c_str(), X_OK) != 0)
		throw "invalid path: " + val1;
	
}

void Location::setroot(std::string &str)
{
	if (!root.empty())
		throw "multiple root location" + str;
	trim(str, "\n\t; ");
	char *tmp = realpath(str.c_str(), nullptr);
	if (tmp == nullptr)
	{
		throw "invalid path: " + str;
	}
	root = tmp;
	urldecoded(root);
	delete []tmp;
}

void Location::setautoindex(std::string &str)
{
	trim(str, "\n\t; ");
	if (str == "on")
		autoindex = 1;
	else if (str == "off")
		autoindex = 0;
	else
		throw "invalid syntax in autoindex" + str;
}


void Location::setmethod(std::string &str)
{
	method = 0;
	while (!str.empty())
	{
		trim(str, "\n\t ;");
		if (str.empty())
			return;
		std::string tmp = str.find_first_of(" \n\t") == str.npos? str: str.substr(0, str.find_first_of(" \n\t"));
		str = str.find_first_of(" \n\t") == str.npos? "": str.substr(str.find_first_of(" \n\t"));
		if (tmp != "GET" && tmp !=  "POST" && tmp != "DELETE")
		{
			throw "invalid methods argument" + str;
		}
		method |= ((tmp == "GET") + 2*(tmp == "POST") + 4 * (tmp == "DELETE"));
	}
}


void Location::parsLine(std::string &str)
{
	trim(str, "; \t\n");
	if (str.find_first_of("\n\t ") == str.npos)
		throw "expected experation" + str;
	std::string tmp = str.substr(0, str.find_first_of("\n\t "));
	str = str.substr(str.find_first_of("\n\t "));
	if (str.find("#") != str.npos)
		throw "unexpected comment befor ;" + str;
	trim(str, " \t\n;");
	trim(tmp, " \t\n;");
	if (func.find(tmp) == func.end())
	{
		throw "invalid syntax: " + tmp;
	}
	(this->*func[tmp])(str);
}

void Location::pars(std::string &str)
{
	if (str.empty())
		return ;
	trim(str, "\n\t ");
	if (str[0] == '#')
	{
		if (str.find('\n') == str.npos)
			return ;
		str = str.substr(str.find('\n') + 1);
		return pars(str);
	}
	if (str.find(';') == str.npos)
		throw "expected ; in location" + str;
	std::string tmp = str.substr(0, str.find(';'));
	parsLine(tmp);
	str = str.substr(str.find(';') + 1);
	trim(str, " \n\t");
	pars(str);
}




void Location::setlocation(const std::string &str)
{
	location = str;
}
std::string Location::getlocation()
{
	return location;
}


std::string Location::getroot()
{
	return root;
}

int Location::getmethod()
{
	return method;
}

int Location::getautoindex()
{
	return autoindex;
}

std::string Location::getindex()
{
	return index;
}

int		Location::isCgi()
{
	return cgi;
}




void    Location::urldecoded(std::string &url)
{
    std::string tmp(url);
    int j = 0;
    for (size_t i = 0; i < url.length(); i++)
    {
        if (url[i] == '%')
        {
            if (i + 2 < url.length() && isxdigit(url[i+1]) && isxdigit(url[i+2]))
            {
                tmp[j] = hexToChar(url.substr(i+1, 2));
                i += 2;
            } 
            else
                throw "error url: " + url;
        }
        else
            tmp[j] =url[i];
        j++;
    }
    tmp.resize(j);
    url = tmp;
}

Location::~Location()
{
}
