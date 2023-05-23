/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerPort.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 10:20:44 by nhanafi           #+#    #+#             */
/*   Updated: 2023/05/21 02:28:17 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerPort.hpp"
#include <sys/stat.h>
ServerPort::ServerPort(std::string &input, const std::string &tp):types(tp)
{
    std::string  str;
    std::ifstream file(input);

    if (!file.is_open())
        throw "error file: " + input;
    std::getline(file, str, '\0');
    addServer(str);
    file.close();
}


void ServerPort::parsServer(std::string &str)
{
    Config *tmp = new Config(str);
    sers.insert(std::make_pair(tmp->getip() + ":" + tmp->getport(), tmp));
    ports.insert(tmp->getport());
}

std::set<std::string> ServerPort::getports()
{
    return ports;
}

Config *ServerPort::getserver(std::string port, std::string ip, std::string name)
{
    std::pair<std::multimap < std::string, Config *>::iterator, std::multimap < std::string, Config *>::iterator> range = sers.equal_range(ip+":"+port);
    if (range.first == range.second)
        return sers.begin()->second;
    for (std::multimap < std::string, Config *>::iterator i = range.first; i != range.second; i++)
    {
        if (i->second->getname() == name)
            return i->second;
    }
    return range.first->second;
}


std::string findend(std::string &str)
{
    int bk = 1;
    int i = 1;
    int j = 0;
    std::string res;

    j = str.find('{');
    if ((size_t)j == std::string::npos)
        throw "error breaket1" + str;
    for (; (size_t)j + i < str.size(); ++i)
    {
        if (str[i + j] == '#')
        {
            if (str.find_first_of("\n", i + j) == std::string::npos)
                throw "error breaket2" + str;
            i = str.find_first_of("\n", i + j) - j;
        }
        if (str[j + i] == '{')
            bk++;
        else if (str[j + i] == '}')
            bk--;
        if (!bk)
            break;
    }
    if ((size_t)j + i >= str.size())
        throw "error breaket2" + str;
    res = str.substr(j + 1, i - 1);
    str = str.substr(i + j + 1);
    return res;
}


void ServerPort::addServer(std::string &str)
{
    if (str.empty() || str.find_first_not_of("\n \t") == std::string::npos)
        return;
    str = str.substr(str.find_first_not_of("\n \t"));
    if (str.empty())
        return;
    if (str[0] == '#')
    {
        if (str.find_first_of("\n") == std::string::npos)
            return;
        str = str.substr(str.find_first_of("\n"));
        return addServer(str);
    }
    if(str.find("server") != 0)
    {
        throw "error no server";
    }
    if (str.find("{") == std::string::npos || str.substr(6, str.find("{") - 6).find_first_not_of(" \n") != std::string::npos)
        throw "error expected brakets '{}'";
    std::string serv = findend(str);
    parsServer(serv);
    addServer(str);
}


void ServerPort::setresponse(Request &req)
{
    std::string tp = req.getUrl().substr(req.getUrl().find_last_of(".") + 1);
    req.setcontent_type(types.gettype(tp));
    req.response.url = req.getUrl();
    req.response.body_exist = 1;
    if (req.range.first >= 0)
    {
        req.setStatus(206);
        req.setstatustring("Partial Content");
        if (req.range.first < 0)
            req.range.first = 0;
        if (req.range.second <= req.range.first)
            req.range.second = req.range.first + 3e6;
        req.response.total_range = req.range;   
    }
    else
    {
        req.setStatus(200);
        req.setstatustring("Ok");
	    req.response.total_range = std::make_pair(0, -1);
    }
    req.reshandler();
}

void ServerPort::cgihandler(Request &req, Config *tmp, Location *loc)
{
    pid_t  pid;
    int    filedes[2], pipes[2];

    if (access(req.getUrl().c_str(), F_OK) != 0)
    {
        req.setStatus(404);
        req.setstatustring("Not Found");
        tmp->geterror(req, types);
        return;
    }
    pipe(filedes);
    pid = fork();
    if (pid == -1)
    {
        req.setStatus(500);
        req.setstatustring("Internal Server Error");
        tmp->geterror(req, types);
        return;
    }
    if (pid == 0)
    {
        std::string                         cmd, filename, options;
        std::vector<const char*>            arg;
        std::vector<const char*>                  execve_env;
        execve_env = req.getcgienv();
        execve_env.push_back(NULL);
        if (req.getUrl().find_last_of(".") == std::string::npos\
        || loc->cgi_cmd.find(req.getUrl().substr(req.getUrl().find_last_of("."))) == loc->cgi_cmd.end())
        {
            req.setStatus(500);
            req.setstatustring("Internal Server Error");
            tmp->geterror(req, types);
            return;
        }
        arg.push_back(loc->cgi_cmd[req.getUrl().substr(req.getUrl().find_last_of("."))].first.c_str());
        if (!loc->cgi_cmd[req.getUrl().substr(req.getUrl().find_last_of("."))].second.empty())
            arg.push_back(loc->cgi_cmd[req.getUrl().substr(req.getUrl().find_last_of("."))].second.c_str());
        arg.push_back(strdup(req.getUrl().c_str()));
        if (!req.getquerystring().empty())
            arg.push_back(strdup(req.getquerystring().c_str()));
        arg.push_back(NULL);
        pipe(pipes);
        pid = fork();
        if (pid == -1)
            exit(1);
        if (pid == 0)
        {
            dup2(pipes[1], 1);
            close(pipes[0]);
            close(pipes[1]);
            for (t_content *c = req.getcontent(); c != NULL; c = req.getcontent())
            {
                std::cout<< c->body;
            }
            exit(0);
        }
        dup2(pipes[0], STDIN_FILENO);
        close(pipes[1]);
        close(pipes[0]);
        dup2(filedes[1], 1);
        close(filedes[0]);
        close(filedes[1]);
        if (execve(arg[0], (char *const *)&arg[0], (char *const *)&execve_env[0]) == -1)
        {
            exit(1);
        }
        exit(1);
    }
    close(filedes[1]);
    req.response.iscgi = 1;
    req.response.cgifd = filedes[0];
    req.response.cgipid = pid;
}



void    ServerPort::respond(Request &req)
{
    Config *tmp = getserver(req.getport(), req.getip(), req.getservername());
    if (req.getHeaderrsp())
        return tmp->geterror(req, types);
    Location *loc = tmp->getLocation(req.getUrl());
    if (!loc)
    {
        req.setStatus(404);
        req.setstatustring("Not F|ound");
        tmp->geterror(req, types);
        return;
    }
    if (req.getcontent_length() > tmp->getclient_max_body_size())
    {
        req.setStatus(413);
        req.setstatustring("Request Entity Too Large");
        tmp->geterror(req, types);
        return;
    }
    req.modifUrl(loc->getlocation(), loc->getroot());
    if (req.getHeaderrsp())
        return tmp->geterror(req, types);
    switch ((loc->getmethod() & req.getmethod()))
    {
        case 1:
            getGet(req, tmp, loc);
            break;
        case 2:
            getPost(req, tmp, loc);
            break;
        case 4:
            getDelete(req, tmp);
            break;
        default:
            req.setStatus(405);
            req.setstatustring("Method Not Allowed");
            tmp->geterror(req, types);
    }
    req.reshandler();
}


void  ServerPort::getGet(Request &req, Config *tmp, Location *loc)
{
    if (access(req.getUrl().c_str(), F_OK) != 0)
    {
        req.setStatus(404);
        req.setstatustring("Not Found");
        tmp->geterror(req, types);
        return;
    }
    else if (isdir(req.getUrl()) == 1)
    {

        if (loc->getautoindex())
        {
            if (req.getpath()[req.getpath().size() - 1] != '/')
            {
                req.setStatus(301);
                req.setstatustring("Moved Permanently");
                req.setLocation(req.getpath() + "/");
                tmp->geterror(req, types);
                return;
            }
            req.setUrl(req.getUrl() + loc->getindex());
            if (isdir(req.getUrl()) == - 1)
            {
                req.setStatus(404);
                req.setstatustring("Not Found");
                tmp->geterror(req, types);
                return;
            }
            if (isdir(req.getUrl()) == 1)
            {
                req.setStatus(200);
                req.setstatustring("OK");
                req.response.body = generateHTMLPage(req.getUrl(), req.getpath());
                req.response.body_exist = 1;
                req.setcontent_lengh(req.response.body.size());
                return req.reshandler();
            }
        }
        else
        {
            req.setStatus(403);
            req.setstatustring("Forbidden");
            tmp->geterror(req, types);
            return;
        }
    }
    if (loc->isCgi() && loc->cgi_cmd.find(req.getUrl().substr(req.getUrl().find_last_of("."))) != loc->cgi_cmd.end())
        return cgihandler(req, tmp, loc);
    setresponse(req);
    
}

void   ServerPort::creatfiles(Request &req, Config *tmp)
{
    for (t_content *c = req.getcontent(); c != NULL; c = req.getcontent())
    {
        std::string url = req.getUrl() + c->filename;
        std::ofstream outputFile(url);
        if (outputFile.is_open())
        {
            outputFile << c->body;
            outputFile.close();
        }
        else 
        {
            req.setStatus(403);
            req.setstatustring("Forbidden");
            tmp->geterror(req, types);
        }

    }
    req.setStatus(201);
    req.setstatustring("Created");
    tmp->geterror(req, types);
}

void  ServerPort::getPost(Request &req, Config *tmp, Location *loc)
{
    if (access(req.getUrl().c_str(), F_OK) != 0)
    {
        req.setStatus(404);
        req.setstatustring("Not Found");
        tmp->geterror(req, types);
        return;
    }
    if (isdir(req.getUrl()) != 1)
    {
        if (loc->isCgi())
             return cgihandler(req, tmp, loc);
        req.setStatus(403);
        req.setstatustring("Forbidden");
        tmp->geterror(req, types);
        return;
    }
    if (req.getUrl()[req.getUrl().size() - 1] != '/')
    {
        req.setStatus(301);
        req.setstatustring("Moved Permanently");
        req.setLocation(req.getpath() + "/");
        tmp->geterror(req, types);
        return;
    }
    if (loc->isCgi() && loc->getautoindex())
    {
        req.setUrl(req.getUrl() + loc->getindex());
        return cgihandler(req, tmp, loc);
    }
    if (req.getcompletbody())
        return creatfiles(req, tmp);
    req.setStatus(400);
    req.setstatustring("Bad Request");
    
}

void  ServerPort::getDelete(Request &req, Config *tmp)
{
     if (access(req.getUrl().c_str(), F_OK) != 0)
    {
        req.setStatus(404);
        req.setstatustring("Not Found");
        tmp->geterror(req, types);
        return;
    }
    else if (isdir(req.getUrl()) == 1)
    {
        if (req.getUrl()[req.getUrl().size() - 1] != '/')
        {
            req.setStatus(409);
            req.setstatustring("Conflict");
            req.setLocation( req.getUrl() + "/");
            tmp->geterror(req, types);
            return;
        }
        if (!remove_directory(req.getUrl().c_str()))
        {
            req.setStatus(403);
            req.setstatustring("Forbidden");
            tmp->geterror(req, types);
            return;
        }
    }
    else
    {
        if (unlink(req.getUrl().c_str()))
        {
            req.setStatus(403);
            req.setstatustring("Forbidden");
            tmp->geterror(req, types);
            return;
        }
    }
    req.setStatus(204);
    req.setstatustring("No Content");
}
ServerPort::~ServerPort()
{
    for (std::multimap < std::string, Config *>::iterator 
    it = sers.begin(); it != sers.end(); it++)
    {
        delete it->second;
    }
    
}