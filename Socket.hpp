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
	void listen();
	int accept();
	int read(int client_sock, char* buffer, size_t size);

private:
	int sock_fd;
	struct sockaddr_in serv_addr;
};


#endif /* ********************************************************** SOCKET_H */
