/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 23:24:18 by nhanafi           #+#    #+#             */
/*   Updated: 2023/03/15 10:38:04 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void ltrim(std::string &s, std::string w)
{
    size_t start = s.find_first_not_of(w);
    s = (start == std::string::npos) ? "" : s.substr(start);
}
 
void rtrim(std::string &s, std::string w)
{
    size_t end = s.find_last_not_of(w);
    s =  (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
void trim(std::string &s, std::string w)
{
    rtrim(s,w);
    ltrim(s,w);
}