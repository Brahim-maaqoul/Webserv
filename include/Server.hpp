/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 10:39:29 by nhanafi           #+#    #+#             */
/*   Updated: 2023/05/08 18:12:47 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
#include "ServerPort.hpp"
# include "Socket.hpp"
# include "Request.hpp"
# include <vector>
# include <map>
# include <fcntl.h>
# include <stdbool.h>
#include <fstream>
#include <sstream>
#include <sys/ioctl.h>
#include <filesystem>
#include <utility>
#include <sys/stat.h>
#include <sys/socket.h>

// #include <sys/epoll.h>

#include <queue>

class Socket;


class Server
{
    private:
        struct s_fds
        {
            std::queue <std::pair<Request *, int> > resev;
            std::queue <std::pair<Request *, int> > send;
        };
        std::map <int , Request *> _req;
        std::vector<Socket> _sockets;
        std::set<std::string> _port;
        ServerPort      _config;
        std::vector<struct pollfd>  fds;
    public:

        void    serverloop();
        Server(ServerPort &config);
        void    pullin(size_t idx);
        void    pullout(size_t idx);
};
#endif