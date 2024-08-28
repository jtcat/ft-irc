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

class Server
{
public:
	friend class MessageParser;
	Server(int port, const std::string &passwd);
	~Server(void);

	void bind(void);
	void listen(int n);
	const Client accept(void);
	int getSockFd(void);
	static int send(Client *client, const std::string &msg);
	void monitorClients(void);
	void addPollFd(int client_fd);
	void delPollFd();
	void registerNewClient();
	void closeClientConnection(int client_fd);
	void addChannel(Channel * channel);

	//	int read(int client_sock, char* buffer, size_t size);
private:
	size_t _poll_i;
	int _sock_fd;
	struct sockaddr_in _serv_addr;
	std::vector<struct pollfd> _pfds;
	std::string _passwd;
	int _fd_count;
	std::map<std::string, Channel *> _channels;
	std::map<int, Client *> _clients;
};
