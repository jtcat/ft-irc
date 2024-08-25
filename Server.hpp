#pragma once

#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "Channel.hpp"
#include "Client.hpp"

class Server {
	public:
		Server(int port);
		~Server(void);

		void	bind(void);
		void	listen(int n);
		const Client	accept(void);
		int		getSockFd(void);
		void	monitorClients(void);
		void	addPollFd(int client_fd);
		void	delPollFd(int i);
		void	broadcast(int sender_fd, char *msg, int nbytes);
		void	registerNewClient();
		//	int read(int client_sock, char* buffer, size_t size);

	private:
		int									_sock_fd;
		struct sockaddr_in 					_serv_addr;
		std::vector<struct pollfd> 			_pfds;
		int 								_fd_count;
		std::map<std::string , Channel *> 	_channels;
		std::map<int ,Client *> 	_clients;
};
