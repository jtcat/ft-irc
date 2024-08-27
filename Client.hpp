#pragma once

#include <iostream>
#include <string>
#include <map>

class Channel;
#include "Channel.hpp"

class Client
{
public:
	// friend class MessageParser;
	Client(int sock_fd, std::string ip_addr);
	Client(Client const &src);
	~Client(void);
	int getSockFd() const;
	const std::string &getIpAddr() const;
	const std::string &getNick() const;
	const std::string &getUsername() const;
	int getAuthenticatedFlag() const;
	int getRegisteredFlag() const;
	void setAuthenticatedFlag(int flag);
	void setRegisteredFlag(int flag);
	void setUsername(const std::string &username);
	void setNick(const std::string &nick);
	void setRealname(const std::string &realname);
	const std::map<std::string, Channel *> &getChannels() const;\
	void addChannel(std::map<std::string, Channel *>::iterator &it);
	bool isUserMemberOfChannel(const std::string &channel) const;
	Client &operator=(Client const &rhs);
	friend std::ostream& operator<<(std::ostream& os, const Client& client);


private:
	Client(void);
	int _sock_fd;
	std::string _ip_addr;
	std::string _nick;
	std::string _username;
	std::string _realname;
	int _authenticated;
	int _registered;
	std::map<std::string, Channel *> _channels;
};
