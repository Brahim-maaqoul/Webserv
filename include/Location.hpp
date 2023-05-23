/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 00:35:23 by nhanafi           #+#    #+#             */
/*   Updated: 2023/05/21 01:14:51 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <string>
# include <map>
# include <utility>
# include "utils.hpp"
# include "Request.hpp"

class Location
{

	private:
		typedef void (Location::*method_function)(std::string&);
		bool				matching;
		std::string			root;
		bool				autoindex;
		std::string			location;
		short int			method;
		std::string			index;
		int					cgi;
		std::string			cgitype;
	public:
		std::map<std::string , std::pair<std::string, std::string> > cgi_cmd;
		Location();
		Location(std::string &str);
		~Location();
		std::map<std::string, method_function> func;
		void 		mapinit();
		void 		parsLine(std::string &str);
		void 		pars(std::string &str);
		void 		setroot(std::string &str);
		void 		setlocation(const std::string &str);
		void 		setautoindex(std::string &str);
		void 		setmethod(std::string &str);
		void 		setcgi(std::string &str);
		void 		setindex(std::string &str);
		std::string getlocation();
		std::string getroot();
		int 		getmethod();
		int 		getautoindex();
		std::string getindex();
		int			isCgi();
		void		urldecoded(std::string &url);
};

#endif