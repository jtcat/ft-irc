#include "Client.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client()
{
}

Client::Client(const Client &src)
{
	*this = src;
}

Client::Client(int sock_fd, std::string ip_addr) : _sock_fd(sock_fd), _ip_addr(ip_addr), _nick("*") ,_username("") , _authenticated(0), _registered(0) {}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
	// when a client is deleted we need to erase its entry from the list of users
	// of all chanels from which that client was part of
	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
		it->second->delUser(_nick);
	close(_sock_fd);
	// std::cout << "Destroying Client with ip = " << _ip_addr <<" and fd = " << _sock_fd<< std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client &Client::operator=(Client const &rhs)
{
	if (this != &rhs)
	{
		_sock_fd = rhs.getSockFd();
		_ip_addr = rhs.getIpAddr(); // Assuming _ip_addr is a pointer to a string literal or managed elsewhere
		_nick = rhs.getNick();
		_username = rhs.getUsername();
		_authenticated = rhs.getAuthenticatedFlag();
		_channels = rhs.getChannels();
	}
	// std::cout << "Copying Client with ip = " << _ip_addr <<" and fd = " << _sock_fd<< std::endl;
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

const std::string &Client::getIpAddr() const
{
	return _ip_addr;
};

const std::string &Client::getNick() const
{
	return _nick;
}

const std::string &Client::getUsername() const
{
	return _username;
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

void Client::setAuthenticatedFlag(int flag) {
	_authenticated = flag;
};
void Client::setRegisteredFlag(int flag) {
	_registered = flag;
};
void Client::setUsername(const std::string &username)  {
	_username = username;
};
void Client::setNick(const std::string &nick) {
	_nick = nick;
};
void Client::setRealname(const std::string &realname) {
	_realname = realname;
};
std::ostream& operator<<(std::ostream& os, const Client& client) {
    os << "Nick: " << client._nick << "\n"
       << "User: " << client._username << "\n"
       << "Host: " << client._ip_addr << "\n";
    // Print other members as needed...
    return os;
}
/* ************************************************************************** */
