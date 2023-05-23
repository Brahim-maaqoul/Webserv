/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mimepars.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 21:19:35 by nhanafi           #+#    #+#             */
/*   Updated: 2023/05/21 01:44:35 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mimepars.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Mimepars::Mimepars(std::string fl)
{
	std::ifstream File(fl);
	if (!File.is_open())
		throw " no such file or directory: 'mime.types'";
	std::string str;
	getline(File, str, '\0');
	settypes(str);
	File.close();
	
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Mimepars::~Mimepars()
{
}


/*
** --------------------------------- METHODS ----------------------------------
*/

std::string Mimepars::gettype(const std::string &str)
{
	if (mp.find(str) == mp.end())
		return "text/html";
	return mp[str];
}

void		Mimepars::settype(std::string &str)
{
	trim(str, "\n\t ;");
	if (str.find_first_of(" \n\t") ==  str.npos)
		throw "no enough argumet in mime.type" + str;
	std::string value = str.substr(0, str.find_first_of("\n\t "));
	str = str.substr(str.find_first_of("\n\t "));
	trim(str, "\n\t ;");
	while(!str.empty())
	{
		std::string key = str.find_first_of("\n\t ") != str.npos ? str.substr(0, str.find_first_of("\n\t ")) :str;
		str = str.find_first_of("\n\t ") != str.npos ? str.substr(str.find_first_of("\n\t ")) : "";
		mp[key] = value;
		trim(str, "\n\t ;");
	}

}

void		Mimepars::settypes(std::string &str)
{
	trim(str, "\n\t ");
	if (str.empty())
		return ;
	if (str.find(';') == str.npos)
		throw "Error mime.types" + str;
	std::string tmp = str.substr(0, str.find(';'));
	str = str.substr(str.find(';') + 1);
	settype(tmp);
	settypes(str);
}