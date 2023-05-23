/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerPort.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 10:21:31 by nhanafi           #+#    #+#             */
/*   Updated: 2023/05/21 01:34:07 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERPORT_HPP
# define SERVERPORT_HPP

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <fstream>
#include <string>
#include <filesystem>
#include "Config.hpp"
#include <filesystem>

class ServerPort
{
    private:
        std::multimap < std::string, Config *> sers;
        std::set <std::string> ports;
        Mimepars types;
    public:
        ServerPort(std::string &input, const std::string &tp);
        ~ServerPort();
        Config *getserver(std::string port,std::string ip, std::string name);
        void    respond(Request &req);
        void addServer(std::string &str);
        void parsServer(std::string &str);
        void setresponse(Request &req);
        std::set <std::string> getports();
        void getGet(Request &req, Config *tmp, Location *loc);
        void getPost(Request &req, Config *tmp, Location *loc);
        void getDelete(Request &req, Config *tmp);
        void creatfiles(Request &req, Config *tmp);
        void cgihandler(Request &req, Config *tmp, Location *loc);
};
int isDirectory(std::string path);
#endif