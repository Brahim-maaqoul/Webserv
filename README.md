# WebServ 42 Project

WebServ is a project developed as part of the curriculum at 42, a coding school. It is a web server implementation written in C++98, designed to handle basic HTTP requests and serve web pages.

## Getting Started

To get started with the WebServ project, follow these instructions:

1. Clone the repository: `git clone https://github.com/Brahim-maaqoul/Webserv`
2. Change directory to the project folder: `cd Webserv`
3. Build the project: `make`
4. Run the server: `./Server`

## Usage

Once the server is running, it will listen for incoming HTTP requests on the specified port (default is port 8080). You can access the server using a web browser or by sending HTTP requests using tools like `curl`.

## Features

WebServ supports the following features:

- Handling HTTP GET requests to serve static web pages.
- MIME type detection for common file types (HTML, CSS, JavaScript, images, etc.).
- Multi-threading to handle multiple concurrent connections.
- Basic error handling and logging.

## Configuration

The server can be configured by modifying the `config.cfg` file. The following parameters can be adjusted:

- `PORT`: The port number on which the server listens for incoming connections.
- `DOCUMENT_ROOT`: The root directory where the web pages are stored.
- `DEFAULT_FILE`: The default file to serve when a directory is requested.

## Contributing

Contributions to the WebServ project are welcome! If you find any bugs or have suggestions for improvements, please open an issue or submit a pull request.



## Acknowledgments

We would like to thank the 42 coding school for providing the opportunity to work on this project and learn about web server implementation.
