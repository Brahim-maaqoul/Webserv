/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 10:39:11 by nhanafi           #+#    #+#             */
/*   Updated: 2023/05/21 02:02:37 by nhanafi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <map>
#include <sys/socket.h>
#include "utils.hpp"
#include <algorithm>
#include <filesystem>
#include <sys/stat.h>

typedef struct s_content
{
	std::string content_disposition;
	std::string name;
	std::string filename;
	std::string content_type;
	std::string body;
} t_content;

typedef struct s_resp
{
	std::string	header;
	std::string	body;
	std::string	url;
	bool		body_exist;
	bool		header_send;
	std::string location;
	long long	sendindex;
	int			iscgi;
	int			cgifd;
	int			cgipid;
	std::pair<long long,long long> total_range;
} t_resp;


class Request
{
	private:
		typedef void (Request::*method_function)(std::string&);

		std::string res;
		size_t index;
		size_t		icont;
		std::string ip;
		std::string server_name;
		std::string port;
		std::string url;
		std::string http_version;
		std::string method;
		std::string body;
		std::string accept_;
		std::string accept_language;
		std::string connection;
		std::string content_type;
		std::string boundary;
		unsigned long long content_length_body;
		unsigned long long content_length;
		int status;
		std::string statustring;
		int completebody;
		int completheader;
		std::vector <t_content> content;
		std::map<std::string, method_function> func;
		std::map<std::string, std::string> cookies;
		std::vector <const char *>  cgi_env;
		std::string query_string;
		std::string				path;
	public:
		t_resp response;
		std::pair <long long, long long> range;
		Request();
		void		parsheader(std::string &str);
		void		mapinit();
		std::string	getport();
		std::string getpath()
		{
			return path;
		}
		std::string	getbody()
		{
			return body;
		}
		std::string	getquerystring()
		{
			return query_string;
		}
		void		getfirstline(std::string &str);
		void		headhandler(std::string &str);
		void		contenthandler(std::string &str);
		void		bodyhandler();
		void		setHost(std::string &str);
		void		setType(std::string &str);
		void		setLenght(std::string &str);
		void		setRange(std::string &str);
		void		setCookie(std::string &str);
		void		setStatus(int st);
		void		setstatustring(const std::string &str);
		void		setUrl(const std::string &str);
		void		setmethod(const std::string &str);
		void		sethttp_version(const std::string &str);
		void		error(int type, std::string str);
		std::string getUrl(void);
		std::string getip(void);
		size_t		getcontent_length();
		std::string getservername(void);
		void		content_parts(std::string &str, t_content &c);
		int			getStatus(void);
		int			getmethod(void);
		void		check_request();
		void		modifUrl(std::string matching, std::string location);
		void        setcontent_type(const std::string &str);
		void		addbody(const std::string &str);
		int			getcompletbody();
		int			getcomplethead();
		std::string	getcontent_type();
		t_content	*getcontent();
		int			sendresponse(int fd);
		void		reshandler();
		void		setLocation(const std::string &str);
		void		addCgiEnv(const std::string &key, const std::string &val);
		std::vector<const char *> &getcgienv();
		void		setcontent_lengh(size_t a)
		{
			content_length_body = a;
			content_length = a;
		}
		int 		getHeaderrsp(){
			return status;
		};
		void		testo();
		void	urldecoded(std::string &url);
		~Request();
};

#endif
