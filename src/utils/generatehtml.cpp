/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generatehtml.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 22:12:48 by nhanafi           #+#    #+#             */
/*   Updated: 2023/05/21 00:28:21 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string generateHTMLPage(const std::string& directoryPath, const std::string& uri)
{
    std::string output;

    DIR* directory = opendir(directoryPath.c_str());
    if (!directory)
    {
        return "";
    }

    output += "<html>\n";
    output += "<head><title>"+uri+"</title></head>\n";
    output += "<body>\n";
    output += "<h1>"+uri+"</h1>\n";
    output += "<ul>\n";

    struct dirent* entry;
    while ((entry = readdir(directory)))
    {
        std::string fileName = entry->d_name;
        std::string filePath = uri + fileName;

        if (entry->d_type == DT_REG)
            output += "<li><a href=\"" + filePath += "\">" + fileName + "</a></li>\n";
        else if (entry->d_type == DT_DIR && fileName != "." && fileName != "..")
            output += "<li><a style=\"color:green;font-size:25px\" href=\"" + filePath += "/\">" + fileName + "/</a></li>\n";
    }

    output += "</ul>\n";
    output += "</body>\n";
    output += "</html>\n";

    closedir(directory);
    return output;
}
