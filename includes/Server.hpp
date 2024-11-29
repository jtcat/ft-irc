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

#define SERVER_MOTD "	<<< Welcome to the FT_IRC server! >>> \n"

class Server {
	public:
		friend class MessageParser;
		Server(int port, const std::string &passwd);
		~Server(void);

		void bind(void);
		void listen(int n);
		const Client accept(void);

		int					getSockFd(void);
		const std::string&	getMOTD(void) const;
		const std::string&	getName(void) const;
		const std::string&	getCreateDate(void) const;

		static int send(Client *client, const std::string &msg);

		void monitorClients(void);
		void addPollFd(int client_fd);
		void delPollFd();
		void registerNewClient();
		void closeClientConnection(int client_fd);
		void addChannel(Channel * channel);
		Channel *getChannel(const std::string &channel) const;
		bool ChannelExists(const std::string &channel) const;
		bool UserExists(const std::string &nick) const;
		Client *getClient(const std::string &nick) const;
		const std::string&	getDefaultKickMsg(void) const;
		//	int read(int client_sock, char* buffer, size_t size);
	private:
		const std::string	_name;
		const std::string	_motd;
		const std::string	_create_date;
		const std::string	_default_kick_msg;
		size_t _poll_i;
		int _sock_fd;
		struct sockaddr_in _serv_addr;
		std::vector<struct pollfd> _pfds;
		std::string _passwd;
		int _fd_count;
		std::map<std::string, Channel *> _channels;
		std::map<int, Client *> _clients;
		std::map<std::string, Client *> _client_users;
};
