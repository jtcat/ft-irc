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

const std::map<std::string, Channel *> &Client::getChannels() const
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

void	Client::processMessage(const std::string& msgPart) {
	std::string::size_type	beg, end;
	std::string				subMsg;
	std::string 			msgBuffer = _msg_buffer;

	std::cout << __func__ << "::Debug [0] <_msg_buffer> : " << _msg_buffer << std::endl;

	beg = 0;
	while ((end = msgPart.find("\r\n", beg)) != msgPart.npos) {
		end += 2;
		subMsg = msgPart.substr(beg, end - beg);
		beg = end;
		std::cout << __func__ << "::Debug [1] <subMsg> : " << subMsg << std::endl;
		std::cout << __func__ << "::Debug [2] <end>    : " << end << std::endl;
		if (_msg_buffer.size() > 0) {
			if ((_msg_buffer.size() + subMsg.size()) > _msg_buffer_max_size) {
				_msg_buffer.clear();
				Server::send(this, ERR_INPUTTOLONG(getNick()));
				continue ;
			}
			// _msg_buffer.append(subMsg);
			msgBuffer.append(subMsg);
			std::cout << __func__ << "::Debug [3] <_msg_buffer> : " << _msg_buffer << std::endl;
			std::cout << __func__ << "::Debug [4.1] else ... parseBuffer() done" << std::endl;
			MessageParser::parseBuffer(msgBuffer, this);
			// MessageParser::parseBuffer(_msg_buffer, this);
			std::cout << __func__ << "::Debug [4.2] else ... parseBuffer() done" << std::endl;
			_msg_buffer.clear();
		}
		else {
			std::cout << __func__ << "::Debug [5.1] else ... parseBuffer() done" << std::endl;
			MessageParser::parseBuffer(subMsg, this);
			std::cout << __func__ << "::Debug [5.2] else ... parseBuffer() done" << std::endl;
		}
	}

	if ((beg + 1) < (msgPart.size())) {
		_msg_buffer.append(msgPart.substr(beg, end));
	}
}
