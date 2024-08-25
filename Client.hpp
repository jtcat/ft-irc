#pragma once

#include <iostream>
#include <string>
#include <map>

class Channel;
#include "Channel.hpp"

class Client
{
public:
	Client(int sock_fd, std::string ip_addr);
	Client(Client const &src);
	~Client(void);
	int getSockFd() const;
	const std::string &getIpAddr() const;
	const std::string &getNick() const;
	const std::string &getUsername() const;
	int getAuthenticated() const;
	const std::map<std::string, Channel *> &getChannels() const;
	Client &operator=(Client const &rhs);

private:
	Client(void);
	int _sock_fd;
	std::string _ip_addr;
	std::string _nick;
	std::string _username;
	int _authenticated;
	int _registered;
	std::map<std::string, Channel *> _channels;
};
