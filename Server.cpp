#include "Server.hpp"
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

Server::Server(int port) : _pfds(), _fd_count(0)
{
	_sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (_sock_fd < 0)
	{
		perror("ERROR opening socket");
		exit(1);
	}
	// init the sock_address struct to pass to bind
	bzero((char *)&_serv_addr, sizeof(_serv_addr));
	_serv_addr.sin_family = AF_INET;
	_serv_addr.sin_addr.s_addr = INADDR_ANY;
	_serv_addr.sin_port = htons(port);
	// bind the socket to the port passed as input
	int yes = 1;
	if (setsockopt(_sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
	{
		perror("setsockopt");
		exit(1);
	}
}

Server::~Server()
{
	close(_sock_fd);
}

void Server::bind()
{
	if (::bind(_sock_fd, reinterpret_cast<struct sockaddr *>(&_serv_addr),
			   sizeof(_serv_addr)) < 0)
	{
		perror("ERROR on binding");
		exit(1);
	}
}

int Server::accept()
{
	unsigned int addr_size;
	int newsock_fd;
	struct sockaddr_in client_addr;

	addr_size = sizeof(client_addr);
	newsock_fd = ::accept(
		_sock_fd, reinterpret_cast<struct sockaddr *>(&client_addr), &addr_size);

	if (newsock_fd < 0)
	{
		perror("ERROR on accept");
		exit(1);
	}

	return newsock_fd;
}

void Server::addPollFd(int client_fd)
{
	// std::cout << "Add to Poll fd = "<<client_fd << std::endl;
	struct pollfd temp_pfds;

	temp_pfds.fd = client_fd;
	temp_pfds.events = POLLIN;
	temp_pfds.revents = 0;
	_pfds.push_back(temp_pfds);
	_fd_count++;
}

void Server::delPollFd(int i)
{
	close(_pfds[i].fd);
	_pfds.erase(_pfds.begin() + i);
	_fd_count--;
};

void Server::broadcast(int sender_fd, char *msg, int nbytes)
{
	for (size_t i = 0; i < _pfds.size(); i++)
	{
		int dest = _pfds[i].fd;
		if (dest != _sock_fd && dest != sender_fd)
		{
			if (send(dest, msg, nbytes, 0) == -1)
			{
				perror("send");
			}
		}
	}
}

// void	Server::procMsg(const std::string& buf) {
// 	static	MessageParser	parser;

// 	parser.parseMessage(buf.begin());
// }

void Server::monitorClients()
{
	int client_fd;
	char buff[512];

	this->addPollFd(_sock_fd);
	while (true)
	{
		int poll_count = poll(&_pfds[0], _fd_count, -1);
		if (poll_count == -1)
		{
			perror("poll");
			exit(1);
		}
		for (size_t i = 0; i < _pfds.size(); i++)
		{
			if (_pfds[i].revents & POLLIN)
			{ // We got one!!
				if (_pfds[i].fd == _sock_fd)
				{ // any attempt to connect?
				  // CHECK CONNECTION LIMIT
					client_fd = this->accept();
					this->addPollFd(client_fd);
					// std::cout << "NEW CONNECTION" << std::endl;
				}
				else
				{
					bzero(buff, 512);
					// std::cout << "sizeof buff = " << sizeof(buff)/sizeof(char) << std::endl;
					int bytes_read = recv(_pfds[i].fd, buff, sizeof(buff), 0);
					if (bytes_read <= 0)
					{
						// Got error or connection closed by client
						if (bytes_read == 0)
							std::cerr << "pollserver: socket hung up" << std::endl;
						else
							perror("recv");
						delPollFd(i);
					}
					else
					{
						// procMsg(buff);
						std::cout << buff;
						// std::cout << "ola" << std::endl;
						// broadcast(_pfds[i].fd, buff, bytes_read);
					}
				}
			}
		}
	}
}

void Server::listen(int n)
{
	::listen(_sock_fd, n);
}

int Server::getSockFd()
{
	return _sock_fd;
}
