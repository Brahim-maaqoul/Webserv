/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 15:16:25 by bmaaqoul          #+#    #+#             */
/*   Updated: 2023/05/20 13:43:56 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

int remove_directory(const char *path)
{
    DIR *dir = opendir(path);
    struct dirent *entry;
    char next_path[2048];

    if (dir == NULL)
        return 0;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        if (entry->d_type == DT_DIR)
            remove_directory(next_path);
        else
            unlink(next_path);
    }
    closedir(dir);
    rmdir(path);

    return 1;
}
