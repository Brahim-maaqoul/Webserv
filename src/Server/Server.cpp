/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 01:45:26 by nhanafi           #+#    #+#             */
/*   Updated: 2023/05/21 02:12:16 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <cstdio>
#include <string>

Server::Server(ServerPort &c) :_port(c.getports()),_config(c)
{
    for (std::set<std::string>::iterator it = _port.begin(); it != _port.end(); it++)
    {
        Socket a(*it);
        _sockets.push_back(a.create_socket());
    }
    fds.resize(_sockets.size());
    for (size_t i = 0; i < _sockets.size(); i++)
    {
        fds[i].fd = _sockets[i]._socket_fd;
        fds[i].events = POLLIN | POLLOUT | POLLHUP | O_NONBLOCK;
    }
}



void    Server::pullin(size_t idx)
{
    Request *tmp = _req[fds[idx].fd];
    int nbytes = 0;
    if(!tmp || ioctl(fds[idx].fd, FIONREAD, &nbytes) < 0 || !nbytes)
        return;
    char buf[nbytes];
    nbytes = recv(fds[idx].fd, buf, nbytes, 0);
    tmp->addbody(std::string(buf, buf + nbytes));

    if (tmp->getcompletbody())
    {
        _config.respond(*tmp);
    }
}

void    Server::pullout(size_t idx)
{
    int status;
    Request *tmp = _req[fds[idx].fd];
    if (!tmp || !tmp->getcompletbody())
        return;
    if (tmp->response.iscgi)
    {
        if (waitpid(tmp->response.cgipid, &status, WNOHANG))
        {
            if (status)
            {
                tmp->response.iscgi = 0;
                tmp->setStatus(500);
                tmp->setstatustring("Internal Server Error");
                _config.respond(*tmp);
                return;
            }
            int nbytes;
            ioctl(tmp->response.cgifd, FIONREAD, &nbytes);
            char s[nbytes];
            read(tmp->response.cgifd, s, nbytes);
            tmp->response.body = s;
            tmp->response.body =  "HTTP/1.1 200 OK\r\n" + tmp->response.body;
            tmp->response.header_send = 1;
            tmp->response.body_exist = 1;
            tmp->response.iscgi = 0;

        }
        return;
    }
    if (!tmp->sendresponse(fds[idx].fd))
    {
        close(fds[idx].fd);
        delete _req[fds[idx].fd];
        _req[fds[idx].fd] = NULL;
        fds.erase(fds.begin() + idx);;
        
    }
}

void    Server::serverloop()
{
    int             status;
    
    while (true)
    {

        status = poll(&fds[0], fds.size(), 0);
        if (status < 0 )
            continue;
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & (POLLHUP | POLLNVAL | POLLERR))
            {
                close(fds[i].fd);
                delete _req[fds[i].fd];
                _req[fds[i].fd] = NULL;
                fds.erase(fds.begin() + i);
            }
            else if (_req[fds[i].fd])
            {
                if (fds[i].revents & POLLIN)
                    pullin(i);
                else if (fds[i].revents & POLLOUT)
                    pullout(i);
            }
            else if (fds[i].revents & POLLIN)
            {
                if (status)
                {
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_sockfd = accept(fds[i].fd, (struct sockaddr *)&client_addr, &client_len);
                    if (fcntl(client_sockfd, F_SETFL, O_NONBLOCK) == -1)
                    {
                        return;
                    }
                    if (client_sockfd > 0)
                    {
                        Request *r = new Request();
                        _req[client_sockfd] = r;
                        struct pollfd tmp;
                        tmp.fd = client_sockfd;
                        tmp.events = POLLIN | POLLOUT | POLLHUP | O_NONBLOCK;
                        fds.push_back(tmp);
                    }
                }
            }
        }
    }
}