/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mimepars.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 21:18:36 by nhanafi           #+#    #+#             */
/*   Updated: 2023/04/16 22:54:56 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIMEPARS_HPP
# define MIMEPARS_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <map>
#include <utils.hpp>

class Mimepars
{

	public:

		Mimepars(std::string fl);
		std::string gettype(const std::string &str);
		void settype(std::string &str);
		void settypes(std::string &str);
		~Mimepars();

	private:
		std::map <std::string, std::string> mp;
};
#endif
