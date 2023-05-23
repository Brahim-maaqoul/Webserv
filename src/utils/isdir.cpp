/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isdir.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 17:42:28 by nhanafi           #+#    #+#             */
/*   Updated: 2023/05/16 17:42:55 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

int isdir(const std::string &path)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        return -1;
    }
    if (S_ISDIR(info.st_mode)) 
        return 1;    
    else if (S_ISREG(info.st_mode))
        return 0;
    return -1;
}