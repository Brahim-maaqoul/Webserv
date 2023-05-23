/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 18:37:47 by nhanafi           #+#    #+#             */
/*   Updated: 2023/05/21 00:23:17 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerPort.hpp"
#include "Server.hpp"
#include "Mimepars.hpp"

int main(int argc, char *argv[])
{
    try
    {
        std::string tmp;
        if (argc == 1)
            tmp = "conf/config";
        else if (argc == 2)
            tmp = argv[1];
        else
            throw "invalid arguments" + tmp;
        ServerPort conf(tmp, "conf/mime.types");
        Server serv(conf);
        serv.serverloop();
    }
    catch(const char* msg)
    {
        std::cerr << msg << '\n';
    }
    catch(const std::string msg)
    {
        std::cerr << msg << '\n';
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    } 
}