#pragma once

#include <iostream>
#include <string>
#include <set>

class Client;
#include "Client.hpp"

class Channel
{
public:
	Channel(const std::string &name, Client *op);
	~Channel(void);

	std::set<Client *>::iterator getUserbyNick(const std::string &nick) const;
	std::set<Client *>::iterator getOpbyNick(const std::string &nick) const;
	bool checkIfUserInvited(Client *client);
	void delUser(const std::string &nick);
	const std::string &getName() const;
	const std::set<Client *> &getUsers() const;
	const std::set<Client *> &getOp() const;
	const std::set<Client *> &getInvites() const;
	const std::string &getPasswd() const;
	int getUserLimit() const;
	int getInviteFlag() const;
private:
	std::string			_name;
	std::set<Client *> _users;
	std::set<Client *> _op;
	std::set<Client *> _invites;
	std::string			_passwd;
	int					_user_limit;
	int					_invite_Only_flag;
	Channel(void);
	Channel &operator=(Channel const &rhs);
	Channel(Channel const &src);
};

