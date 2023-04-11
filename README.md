# Welcome to 42 WebServ!

This is a web service implemented in C++ language, using the sockets API and the HTTP protocol to handle client requests and serve dynamic web pages.

To run the program, make sure you have the necessary libraries installed and use the command "make" to compile the program. Then, use the command "./webserv [configuration file]".

The program will listen for incoming client requests and serve dynamic web pages based on the URL requested by the client. You can customize the web pages by modifying the source code and adding your own HTML, CSS, and JavaScript files to the "www" directory.

The program also supports dynamic content generation using CGI scripts. Simply add your scripts to the "cgi-bin" directory and make sure they have the executable permission. The scripts should output the HTTP response in the correct format.

You can access the web pages from a web browser by visiting "http://localhost:[port_number]/[page_name]" where "port_number" is the port number specified when running the program and "page_name" is the name of the HTML file you want to access.

Please note that the program is designed for educational purposes and may not be suitable for use in a production environment.

Enjoy serving web pages with 42 Web Service!
