#include "Server.hpp"
#include "Client.hpp"
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "MessageParser.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static std::string	calcCreateDate(void) {
	const time_t	t = time(NULL);

	return ctime(&t);
}

Server::Server(int port, const std::string &passwd) : _name("ft_irc"), _motd(SERVER_MOTD), _create_date(calcCreateDate()), _poll_i(0), _pfds(), _passwd(passwd), _fd_count(0)
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

const std::string&	Server::getCreateDate(void) const {
	return _create_date;
}

const std::string&	Server::getMOTD(void) const {
	return _motd;
}

const std::string&	Server::getName(void) const {
	return _name;
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

const Client Server::accept()
{
	unsigned int addr_size;
	int newsock_fd;
	struct sockaddr_in client_addr;
	char ip_str[INET_ADDRSTRLEN];

	addr_size = sizeof(client_addr);
	newsock_fd = ::accept(
		_sock_fd, reinterpret_cast<struct sockaddr *>(&client_addr), &addr_size);
	if (newsock_fd < 0)
	{
		perror("ERROR on accept");
		exit(1);
	}
	inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET6_ADDRSTRLEN);
	// std::cout << "ip str in accept = " << ip_str << std::endl;
	return (Client(newsock_fd, std::string(ip_str)));
}

int Server::send(Client *client, const std::string &msg)
{
	const char *msg_c_str = msg.c_str();
	size_t len = strlen(msg_c_str);
	size_t bytes_sent = 0;
	size_t total_bytes_sent = 0;
	while (total_bytes_sent < len)
	{
		bytes_sent = ::send(client->getSockFd(), msg_c_str + total_bytes_sent, len - total_bytes_sent, 0);
		if (static_cast<int>(bytes_sent) == -1)
		{
			perror("send");
			return -1;
		}
		total_bytes_sent += bytes_sent;
	}
	return total_bytes_sent;
}

void Server::addPollFd(int client_fd)
{
	struct pollfd temp_pfds;

	temp_pfds.fd = client_fd;
	temp_pfds.events = POLLIN;
	temp_pfds.revents = 0;
	_pfds.push_back(temp_pfds);
	_fd_count++;
}

void Server::delPollFd()
{
	_pfds.erase(_pfds.begin() + _poll_i);
	_fd_count--;
};

void Server::registerNewClient()
{
	Client *new_client = new Client(this->accept());

	int new_sock_fd = new_client->getSockFd();
	_clients[new_sock_fd] = new_client;
	_client_users.insert(std::make_pair(new_client->getNick(), new_client));
	this->addPollFd(new_sock_fd);
}

void Server::closeClientConnection(int client_fd)
{
	Client *client = _clients[client_fd];
	_clients.erase(client_fd);
	_client_users.erase(client->getNick());
	delete client;
}

void Server::addChannel(Channel *channel)
{
	_channels[channel->getName()] = channel;
}

bool Server::ChannelExists(const std::string &channel) const
{
	if (_channels.find(channel) != _channels.end())
		return true;
	return false;
}
bool Server::UserExists(const std::string &nick) const {
	if (_client_users.find(nick) != _client_users.end())
		return true;
	return false;
}
Client *Server::getClient(const std::string &nick) const {
	std::map<std::string, Client *>::const_iterator it = _client_users.find(nick);
	if (it != _client_users.end())
		return it->second;
	return NULL;
}

Channel *Server::getChannel(const std::string &channel) const
{
	std::map<std::string, Channel *>::const_iterator it = _channels.find(channel);
	if (it != _channels.end())
		return it->second;
	return NULL;
}

void Server::monitorClients()
{
	char buff[512];

	MessageParser::setServer(this);
	this->addPollFd(_sock_fd);

	while (true)
	{
		int poll_count = poll(&_pfds[0], _fd_count, -1);
		if (poll_count == -1)
		{
			perror("poll");
			exit(1);
		}
		_poll_i = 0;
		while (_poll_i < _pfds.size())
		{
			if (_pfds[_poll_i].revents & POLLIN)
			{
				if (_pfds[_poll_i].fd == _sock_fd)
				{
					// CHECK CONNECTION LIMIT
					registerNewClient();
				}
				else
				{
					bzero(buff, 512);
					int bytes_read = recv(_pfds[_poll_i].fd, buff, sizeof(buff), 0);
					if (bytes_read <= 0)
					{
						// Got error or connection closed by client
						if (bytes_read == 0)
							std::cerr << "pollserver: socket hung up" << std::endl;
						else
							perror("recv");
						closeClientConnection(_pfds[_poll_i].fd);
						delPollFd();
					}
					else
					{
						std::cout << "buffer: " << buff << std::endl;
						_clients[_pfds[_poll_i].fd]->processMessage(buff);
					}
				}
			}
			_poll_i++;
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
