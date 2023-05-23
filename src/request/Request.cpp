/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 17:41:54 by bmaaqoul          #+#    #+#             */
/*   Updated: 2023/05/21 02:28:28 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

/******************CONSTRACTOR*******************************************/
Request::Request():index(0),icont(0),ip("127.0.0.1"),port("80"),content_type("text/html"),content_length(0),status(0),range(std::make_pair(-1,-1))
{

    mapinit();
    completebody = 0;
    completheader = 0;
    
}

void Request::mapinit()
{
	func["Host"] = &Request::setHost;
	func["Content-Type"] = &Request::setType;
	func["Content-Length"] = &Request::setLenght;
	func["Range"] = &Request::setRange;
	func["Cookie"] = &Request::setCookie;
    response.body = "";
    response.body_exist = 0;
    response.header_send = 0;
    response.sendindex = 0;
    response.total_range = std::make_pair(0, - 1);
    response.iscgi = 0;
}






/******************DESTROCTOR*******************************************/
Request::~Request()
{
    for (size_t i = 0; i < cgi_env.size(); i++)
        delete []cgi_env[i];
}

/******************PARSER*******************************************/

void Request::getfirstline(std::string &str)
{
    std::string tmp = str.substr(0, str.find_first_of("\n\t"));
    str = str.substr(str.find_first_of("\n\t\r"));
    trim(tmp, " \r\t\n");
    if (std::count(tmp.begin(), tmp.end(), ' ') != 2)
        return error(400, "Bad Request");
    setmethod(tmp.substr(0, tmp.find_first_of(' ')));
    setUrl(tmp.substr(tmp.find_first_of(' ') + 1, tmp.find_last_of(' ') - tmp.find_first_of(' ') - 1));
    sethttp_version(tmp.substr(tmp.find_last_of(' ') + 1));
}

void    Request::parsheader(std::string &str)
{
    if (str.empty())
        return ;
    getfirstline(str);
    std::string head = str.substr(0, str.find("\r\n\r\n"));
    std::string tmp = str.substr(str.find("\r\n\r\n") + 4);
    completheader = 1;
    headhandler(head);
    check_request();
    body = tmp;
    if (body.size() < content_length)
    {
        index = body.size();
        body.resize(content_length);
        completebody = 0;
        return ;
    }
    completebody = 1;
    bodyhandler();
}

void    Request::headhandler(std::string &str)
{
    trim(str, "\n\r ");
    if(str.empty())
        return ;
    std::string line = str.find_first_of("\n\r") == str.npos ? str : str.substr(0, str.find_first_of("\n\r"));
    str = str.find_first_of("\n\r") == str.npos ? "" : str.substr(str.find_first_of("\n\r"));
    if (line.find(":") == line.npos)
        return headhandler(str);
    std::string val = line.substr(0, line.find(":"));
    line = line.substr(line.find(":") + 1);;
    if (func.find(val) != func.end())
        (this->*func[val])(line);
    headhandler(str);
}

/******************SET*******************************************/

void    Request::setmethod(const std::string &str)
{
    addCgiEnv("REQUEST_METHOD", str);
    method = str;
}

void Request::setUrl(const std::string &str)
{
    if (str.find("?") != str.npos)
    {
        url = str.substr(0, str.find("?"));
        query_string = str.substr(str.find("?") + 1);
        addCgiEnv("QUERY_STRING", query_string);
    }
    else
    {
        url = str;
    }
    addCgiEnv("REQUEST_URI", url);
}

void	Request::sethttp_version(const std::string &str)
{
    http_version = str;
}

void    Request::addCgiEnv(const std::string &key, const std::string &val)
{
    std::string tmp = key + "=" + val;
    cgi_env.push_back(strdup(tmp.c_str()));
}

void Request::setCookie(std::string &str)
{
    trim(str, "\n\r ");
    if (str.empty())
        return ;
    addCgiEnv("HTTP_COOKIE", str);
    size_t pos = str.find(";");
    std::string tmp = pos == str.npos ? str : str.substr(0, pos);
    str = pos == str.npos ? "" : str.substr(pos + 1);
    pos = tmp.find("=");
    std::string key = pos == tmp.npos ? tmp : tmp.substr(0, pos);
    std::string value = pos == tmp.npos ? "" : tmp.substr(pos + 1);
    trim(value, "\n\r ");
    trim(key, "\n\r ");
    cookies[key] = value;
    setCookie(str);
}

void Request::setHost(std::string &str)
{
    trim(str, "\n\r ");
    if (str.find(":") !=  str.npos)
    {
        port = str.substr(str.find(":") + 1);
        addCgiEnv("REMOTE_PORT", port);
        addCgiEnv("SERVER_PORT", port);
        str = str.substr(0, str.find(":"));
    }
    if (str.find_first_not_of("0123456789.") == str.npos && std::count(str.begin(), str.end(), '.') == 3)
    {
        ip = str;
    }
    else
    {
        server_name = str;
        addCgiEnv("HTTP_HOST", port);
    }
}

void Request::setRange(std::string &str)
{
    trim(str, "\n\r ");
    str = str.substr(str.find("=") + 1);
    std::string beg = str.substr(0,str.find("-"));
    std::string end = str.substr(str.find("-") + 1);
    cast(beg, range.first);
    if (!end.empty())
        cast(end, range.second);
    else
        range.second = -1;
}

void Request::setType(std::string &str)
{
    trim(str, "\n\r ");
    content_type = str.substr(0, str.find_first_of(";"));
    addCgiEnv("CONTENT_TYPE", content_type);
    trim(content_type, " \n\r\t");
    boundary = str.substr(str.find("boundary=") + 9);
    trim(boundary, " \n\r\t");
}

void Request::setLenght(std::string &str)
{
    trim(str, "\n\r ");
    if (str.find_first_not_of("0123456789") != str.npos)
        return error(400, "Bad Request");
    addCgiEnv("CONTENT_LENGTH", str);
    cast(str, content_length);
    body.resize(content_length);
}

void    Request::bodyhandler()
{
    trim(body, "\n\r \t");
    while (body != "--" && !body.empty())
    {
        if (body.find(boundary) == body.npos)
            return contenthandler(body);
        std::string tmp = body.substr(0, body.find(boundary) - 2);
        contenthandler(tmp);
        body = body.substr(body.find(boundary) + boundary.size());
    }
}

void    Request::content_parts(std::string &str, t_content &c)
{
    std::stringstream ss(str.substr(str.find(";") + 1));
    std::string tmp;
    getline(ss, tmp, ';');
    c.name = tmp.substr(tmp.find("=") + 1);
    trim(c.name, "\"; \r\n");
    getline(ss, tmp);
    c.filename = tmp.substr(tmp.find("=") + 1);
    trim(c.filename, "\"; \r\n");
    addCgiEnv("CONTENT_FILENAME", c.filename);
    addCgiEnv("CONTENT_NAME", c.name);
}

void    Request::contenthandler(std::string &str)
{
    t_content c;
    
    trim(str, "\n\r\t ");
    if (str.empty())
        return ;
    if (str.find("\r\n\r\n") == str.npos)
    {
        c.body = str;
        c.filename = "UNKNOWN";
        content.push_back(c);
        return ;
    }
    c.body = str.substr(str.find("\r\n\r\n") + 4);
    str = str.substr(0, str.find("\r\n\r\n"));
    std::stringstream ss(str);
    std::string s;
    getline(ss, s);
    trim(s, "\r\n ");
    content_parts(s, c);
    getline(ss, s, '\n');
    trim(s, "\r\n ");
    if (s.find(":") == s.npos)
    {
        status = 400;
        statustring = "Bad Request";
        return ;
    }
    c.content_type = s.substr(s.find(":"));
    trim(c.content_type, " \r\n");
    content.push_back(c);
}

void Request::setStatus(int st)
{
    status = st;
}


void   Request::modifUrl(std::string matching, std::string location)
{
    url = location + "/" + url.substr(matching.size());
    char *tmp = realpath(url.c_str(), nullptr);
    if (!tmp)
    {
        status = 404;
        statustring = "Not Found";
        return ;
    }
    std::string s = tmp;
    delete []tmp;
    if (s.find(location) != 0)
    {
        status = 404;
        statustring = "Not Found";
        return ;
    }
    addCgiEnv("PATH", location);
    addCgiEnv("SCRIPT_FILENAME", url);
}


void Request::setLocation(const std::string &str)
{
    response.location = str;
}

void Request::setcontent_type(const std::string &str)
{
    content_type = str;
}

void   Request::addbody(const std::string &str)
{
    size_t i = 0;

    
    if (str.empty())
        return ;
    if (!completheader)
    {
        std::string tmp(str);
        return parsheader(tmp);
    }
    for (; i < str.size() && index < content_length; i++)
    {
        body[index] = str[i];
        index++;
    }
    if (index >= content_length )
    {
        completebody = 1;
        bodyhandler();
    }
}

void Request::setstatustring(const std::string &str)
{
    statustring = str;
}

/******************GET*******************************************/


std::vector<const char *> &Request::getcgienv()
{
    return cgi_env;
}

size_t Request::getcontent_length()
{
    return content_length;
}

std::string Request::getUrl()
{
    return url;
}

std::string Request::getip()
{
    return ip;
}

std::string Request::getport()
{
    return port;
}

std::string Request::getservername()
{
    return server_name;
}

int Request::getmethod()
{
    return (((method == "GET") + 2 *(method == "POST") + 4* (method == "DELETE")));
}

int Request::getStatus()
{
    return status;
}

int Request::getcompletbody()
{
    return completebody;
}
int Request::getcomplethead()
{
    return completheader;
}

std::string Request::getcontent_type()
{
    return content_type;
}

t_content *Request::getcontent()
{
    if (icont >= content.size())
        return NULL;
    icont++;
    return &content[icont - 1];
}

/******************UTILS*******************************************/

char hexToChar(const std::string& str)
{
    std::stringstream ss;
    ss << std::hex << str;
    int hex;
    ss >> hex;

    char c = static_cast<char>(hex);

    return c;
}

void    Request::urldecoded(std::string &url)
{
    std::string tmp(url);
    int j = 0;
    for (size_t i = 0; i < url.length(); i++)
    {
        if (url[i] == '%')
        {
            if (i + 2 < url.length() && isxdigit(url[i+1]) && isxdigit(url[i+2]))
            {
                tmp[j] = hexToChar(url.substr(i+1, 2));
                i += 2;
            } 
            else
                error(400, "Bad Request");
        }
        else
            tmp[j] =url[i];
        j++;
    }
    tmp.resize(j);
    url = tmp;
}

void    Request::check_request()
{
    if (url.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != url.npos)
        return error(400, "Bad Request");
    urldecoded(url);
    if (url.size() > 2048)
        return error(414, "Request-URI Too Long");
    if (method == "POST" && content_length == 0)
        return error(400, "Bad Request");
    path = url;
}

void Request::error(int type, std::string str)
{
    status = type;
    statustring = str;
}

void    Request::reshandler()
{
    struct stat info;
    if (response.body_exist && response.body.empty())
    {
        stat(response.url.c_str(), &info);
        setcontent_lengh(info.st_size);
        std::ifstream file(getUrl(), std::ios::binary);
        if (range.first >= 0)
        {
            file.seekg(range.first, std::ios::beg);
            char tmp[range.second - range.first + 1];
            file.read(tmp, range.second - range.first + 1);
            range.second = file.gcount() + range.first - 1;
            response.sendindex = response.total_range.first;
            response.body = std::string(tmp, tmp + file.gcount());
        }
        else
        {
            response.total_range.second = info.st_size;
            std::stringstream ss;
            char tmp[std::min((long long)3e6, (long long)content_length)];
            file.read(tmp, std::min((long long)3e6, (long long)content_length));
            response.body = std::string(tmp, tmp + file.gcount());
            response.body = ss.str();
        }
        file.close();
    }
    response.header = http_version + " " + std::to_string(status) + " " + statustring + "\r\n";
    response.header += "Accept-Ranges: bytes\r\n";
    response.header += "Server: webserv42/1.0.0\r\n";
    response.header += "Connection: keep-alive\r\n";
    response.header += "Date: "+ get_date_time() +"\r\n";
    if (!response.location.empty())
        response.header += "Connection:" + response.location + "\r\n";
    if (response.body_exist)
    {
        if (range.first >= 0)
        {
            response.header += "Content-Range: bytes " + std::to_string(range.first);
            response.header += "-" + std::to_string(range.second);
            response.header += "/" + std::to_string(content_length_body) + "\r\n";
            content_length = range.second - range.first + 1;
        }
        response.header += "Content-Type: " + content_type  + "\r\n";
        response.header += "Content-Length: " + std::to_string(content_length) + "\r\n\r\n";
    }
}


int Request::sendresponse(int fd)
{
    if(!response.header_send)
        res = response.header;
    else if (res.empty())
    {
        res = response.body;
    }
    int len = send(fd, res.c_str(), res.size() , 0);
    if (len < 0)
        return 1;
    if (!response.header_send)
    {
        response.header_send = 1;
        res = "";
        return 1;
    }
    response.sendindex += len;
    res = res.substr(len);
    if (res.empty())
    {
        if (response.sendindex < response.total_range.second)
        {
            std::ifstream file(getUrl(), std::ios::binary);
            file.seekg(response.sendindex, std::ios::beg);
            char tmp[std::min((long long)3e6,response.total_range.second - response.sendindex)];
            file.read(tmp, std::min((long long)3e6, response.total_range.second - response.sendindex));
            res = std::string(tmp, tmp + file.gcount());
            file.close();
        }
        else
        {
            return 0;
        }

    }
    return 1;
}