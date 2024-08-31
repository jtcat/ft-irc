#pragma once

#include <iostream>
#include <string>
#include <set>
#include <ctime>
class Client;
#include "Client.hpp"

class Channel
{
	friend class MessageParser;

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
	const std::time_t &getCreationTime() const;
	int getTopicRestrictionFlag() const;
	int getUserLimit() const;
	int getInviteFlag() const;
	void setTopicRestrictionFlag(int flag);
	void setUserLimit(int limit);
	void setInviteFlag(int flag);
	void setPasswd(const std::string &passwd);
	void broadcastMsg(const std::string &msg) const;
	const std::string getUsersList() const;
	void addOp(Client *client);
	void addOp(const std::string &nick);
	void delOp(const std::string &nick);
	void addUser(Client *client);
	void delUserFromInvites(Client *client);
	void clearInvitesList();
	void addUserToInvites(Client *client);
	bool isUserOnChannel(const std::string &nick) const;
	bool isUserOp(const std::string &nick) const;
	bool isUserOp(Client *client);

private:
	Channel &operator=(Channel const &rhs);
	Channel(Channel const &src);
	std::string _name;
	std::set<Client *> _users;
	std::set<Client *> _op;
	std::set<Client *> _invites;
	std::string _passwd;
	int _user_limit;
	int _invite_Only_flag;
	std::time_t _creation_time;
	int _topic_restriction;
	Channel(void);
};
