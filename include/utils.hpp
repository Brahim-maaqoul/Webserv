/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 23:26:19 by nhanafi           #+#    #+#             */
/*   Updated: 2023/05/20 23:12:32 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <fstream> 
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <dirent.h>
#include <ctime>
#include <vector>

std::string get_date_time(void);
std::string generateHTMLPage(const std::string& directoryPath, const std::string& uri);
void trim(std::string &s, std::string w);
int remove_directory(const char *path);
int isdir(const std::string &path);
char hexToChar(const std::string& str);
template <typename T> void cast(const std::string &s, T &res)
{
    std::stringstream ss(s);
    ss >> res;
}
#endif