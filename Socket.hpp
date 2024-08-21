#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class Socket
{
public:
	Socket(int port);
	~Socket();
	void bind();
	void listen(int n);
	int accept();
	int getSockFd();
	// int read(int client_sock, char* buffer, size_t size);

private:
	int _sock_fd;
	struct sockaddr_in _serv_addr;
};


#endif /* ********************************************************** SOCKET_H */
