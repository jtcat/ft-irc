#include "Socket.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

Socket::Socket(int port) {
	_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock_fd < 0) {
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

Socket::~Socket() {
	close(_sock_fd);
}

void Socket::bind()
{
	if (::bind(_sock_fd, reinterpret_cast<struct sockaddr *>(&_serv_addr), sizeof(_serv_addr)) < 0) {
		perror("ERROR on binding");
		exit(1);
	}
}

int Socket::accept() {
	unsigned int addr_size;
	int newsock_fd;
	struct sockaddr_in client_addr;

	addr_size = sizeof(client_addr);
	newsock_fd = ::accept(_sock_fd, reinterpret_cast<struct sockaddr *>(&client_addr), &addr_size);
	if (newsock_fd < 0) {
		perror("ERROR on accept");
		exit(1);
	}
	return newsock_fd;
}

void Socket::listen(int n)
{
	::listen(_sock_fd, n);
}
int Socket::getSockFd()
{
	return _sock_fd;
}
