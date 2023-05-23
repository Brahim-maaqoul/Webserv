/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_date_time.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 23:09:27 by nhanafi           #+#    #+#             */
/*   Updated: 2023/05/21 01:44:04 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string get_date_time(void)
{
     std::string str;

     time_t now = time(0);
     tm *gmtm = gmtime(&now);
     char *dt = asctime(gmtm);
     str = dt;
     std::vector<std::string> v(5);
     size_t found;
     for (size_t i = 0; i < 5; i++)
     {

          found = str.find_first_of(" ");
          if (found != std::string::npos)
          {
               v[i] = str.substr(0,found);
               str = str.substr(found + 1, str.size());
          }
     }
     
     found = str.find_first_of("\n");
     if (found != std::string::npos)
     {
          str = str.erase(found);
     }
     return (v[0] + ", " + v[2] + " " + v[1] + " " + str + " " + v[3] + " GMT");
}
