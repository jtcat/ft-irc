#include "Client.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Server.hpp"

#include "ERR.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client() : _msg_buffer_max_size(MSG_BUFFER_MAX_SIZE)
{
}

Client::Client(const Client &src) : _msg_buffer_max_size(MSG_BUFFER_MAX_SIZE)
{
	*this = src;
}

Client::Client(int sock_fd, std::string ip_addr) : _sock_fd(sock_fd), _host(ip_addr), _nick("*"), _user(""), _msg_buffer_max_size(MSG_BUFFER_MAX_SIZE), _authenticated(0), _registered(0) {}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
	// when a client is deleted we need to erase its entry from the list of users
	// of all chanels from which that client was part of
	close(_sock_fd);
	// std::cout << "Destroying Client with ip = " << _host <<" and fd = " << _sock_fd<< std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client &Client::operator=(Client const &rhs)
{
	if (this != &rhs)
	{
		_sock_fd = rhs.getSockFd();
		_host = rhs.getHost(); // Assuming _host is a pointer to a string literal or managed elsewhere
		_nick = rhs.getNick();
		_user = rhs.getUser();
		_authenticated = rhs.getAuthenticatedFlag();
		_channels = rhs.getChannels();
	}
	// std::cout << "Copying Client with ip = " << _host <<" and fd = " << _sock_fd<< std::endl;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
int Client::getSockFd() const
{
	return _sock_fd;
}

const std::string &Client::getHost() const
{
	return _host;
};

const std::string &Client::getNick() const
{
	return _nick;
}

const std::string &Client::getUser() const
{
	return _user;
}

int Client::getRegisteredFlag() const
{
	return _registered;
}

int Client::getAuthenticatedFlag() const
{
	return _authenticated;
}

const std::map<std::string, Channel *, ci_less> &Client::getChannels() const
{
	return _channels;
}

std::string	Client::getSourceStr(void) const {
	return ":" + getNick() + "!~" + getUser() + "@" + getHost();
}

void Client::setAuthenticatedFlag(int flag)
{
	_authenticated = flag;
};

void Client::setRegisteredFlag(int flag)
{
	_registered = flag;
};

void Client::setUser(const std::string &username)
{
	_user = username;
};

void Client::setNick(const std::string &nick)
{
	_nick = nick;
}

void Client::setRealname(const std::string &realname)
{
	_realname = realname;
}

const std::string&	Client::getRealName(void) const {
	return _realname;
}

std::ostream &operator<<(std::ostream &os, const Client &client)
{
	os << "Nick: " << client._nick << "\n"
	   << "User: " << client._user << "\n"
	   << "Host: " << client._host << "\n";
	// Print other members as needed...
	return os;
}

void Client::addChannel(Channel *channel)
{
	_channels[channel->getName()] = channel;
}

bool Client::isUserOnChannel(const std::string &channel_name) const
{
	std::map<std::string, Channel *>::const_iterator	channel = _channels.find(channel_name);

	if (channel != _channels.end()) {
		return channel->second->isUserOnChannel(getNick());
	}
	return false;
}

void Client::broadcastMsg(const std::string &msg, bool broadcastToHimself) const
{
	std::set<Client *>	known_users;

	for (std::map<std::string, Channel *>::const_iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		const std::set<Client *> &new_users = it->second->getUsers();
		known_users.insert(new_users.begin(), new_users.end());
	}
	for (std::set<Client *>::iterator it = known_users.begin(); it != known_users.end(); it++)
	{
		if (*it == this && broadcastToHimself == false)
			continue ;
		Server::send(*it, msg);
	}
}

void Client::clearMsgBuffer() {
    _msg_buffer.clear();
}

void Client::appendToMsgBuffer(const std::string &str) {
	_msg_buffer.append(str);
}

std::string::size_type Client::getMsgBufferSize() {
	return _msg_buffer.size();
}

std::string::size_type Client::getBufferMaxSize() {
	return _msg_buffer_max_size;
}

const std::string & Client::getMsgBuffer() {
	return _msg_buffer;
}
