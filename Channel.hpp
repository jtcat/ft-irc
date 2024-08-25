#pragma once

#include <iostream>
#include <string>
#include <set>

class Client;
#include "Client.hpp"

class Channel
{
public:
	Channel(void);
	~Channel(void);

	std::set<Client *>::iterator getUserbyNick(const std::string &nick) const;
	std::set<Client *>::iterator getOpbyNick(const std::string &nick) const;
	void delUser(const std::string &nick);

private:
	std::set<Client *> _users;
	std::set<Client *> _op;
	Channel &operator=(Channel const &rhs);
	Channel(Channel const &src);
};

