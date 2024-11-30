#include "Channel.hpp"
#include "Server.hpp"
#include <sstream>
#include "RPL.hpp"
#include "ERR.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
}
Channel::Channel(const std::string &name, Client *op) : _name(name), _user_limit(-1) , _invite_Only_flag(0), _creation_time(std::time(0)), _topic_restriction(0) {
	setTopic(NULL, std::string());
	addOp(op);
};

Channel::Channel( const Channel & src )
{
	*this = src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Channel &				Channel::operator=( Channel const & rhs )
{
	if ( this != &rhs )
	{
		_name = rhs.getName();
		_users = rhs.getUsers();
		_op = rhs.getOp();
		_invites = rhs.getInvites();
		_passwd = rhs.getPasswd();
		_user_limit = rhs.getUserLimit();
		_invite_Only_flag = rhs.getInviteFlag();
	}
	return *this;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

std::set<Client *>::iterator Channel::getUserbyNick(const std::string &nick) const
{
	for (std::set<Client *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if ((*it)->getNick() == nick)
			return it;
	}
	return (_users.end());
}

std::set<Client *>::iterator Channel::getOpbyNick(const std::string &nick) const
{
	for (std::set<Client *>::iterator it = _op.begin(); it != _op.end(); it++)
	{
		if ((*it)->getNick() == nick)
			return it;
	}
	return (_op.end());
}

void Channel::delUser(const std::string &nick)
{
	std::set<Client *>::iterator user_it = getUserbyNick(nick);
	std::set<Client *>::iterator op_it = getOpbyNick(nick);

	if (user_it != _users.end())
		_users.erase(user_it);
	if (op_it != _op.end())
		_op.erase(op_it);
}

void Channel::clearInvitesList() {
	_invites.clear();
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const std::string &Channel::getName() const {
	return _name;
}

const std::string &Channel::getTopic() const {
	return _topic;
}

const std::set<Client *> &Channel::getUsers() const {
	return _users;
}

const std::set<Client *> &Channel::getOp() const {
	return _op;
}

const std::set<Client *> &Channel::getInvites() const {
	return _invites;
}

const std::string &Channel::getPasswd() const {
	return _passwd;
}

int Channel::getUserLimit() const {
	return _user_limit;
}

int Channel::getInviteFlag() const {
	return _invite_Only_flag;
}

const time_t &Channel::getCreationTime() const {
	return _creation_time;
}

bool Channel::checkIfUserInvited(Client *client) {
	if (_invites.find(client) != _invites.end())
		return true;
	return false;
}

void Channel::broadcastMsg(const std::string &msg) const {

	for (std::set<Client *>::iterator it = _users.begin(); it != _users.end(); it++)
		Server::send(*it, msg);
}

const std::string Channel::getUsersList() const {
	std::stringstream ss;
	for (std::set<Client *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (_op.find(*it) != _op.end())
			ss << "@";
		ss << (*it)->getNick();
		std::set<Client *>::iterator next_it = it;
		if (++next_it != _users.end())
			ss << " ";
	}
	return ss.str();
}
void Channel::addUser(Client *client) {
	_users.insert(client);
}

void Channel::addOp(Client *client) {
	addUser(client);
	_op.insert(client);
}

void Channel::addOp(const std::string &nick) {
	std::set<Client *>::iterator it_client = getUserbyNick(nick);
	if(it_client != _users.end())
		_op.insert(*it_client);
}

void Channel::delOp(const std::string &nick) {
	std::set<Client *>::iterator it_client = getOpbyNick(nick);
	if (it_client != _op.end())
		_op.erase(it_client);
}

void Channel::delUserFromInvites(Client *client) {
	_invites.erase(client);
}

void Channel::addUserToInvites(Client *client) {
	_invites.insert(client);
}

bool Channel::isUserOnChannel(const std::string &nick) const
{
	if (getUserbyNick(nick) != _users.end())
		return true;
	return false;
}

int Channel::getTopicRestrictionFlag() const {
	return _topic_restriction;
}

void Channel::setTopicRestrictionFlag(int flag) {
	_topic_restriction = flag;
}

void Channel::setUserLimit(int limit) {
	_user_limit = limit;
}

void Channel::setInviteFlag(int flag) {
	_invite_Only_flag = flag;
}

void Channel::setPasswd(const std::string &passwd) {
	_passwd = passwd;
}

void Channel::sendTopicWhoMsg(Client *client) const {
	Server::send(client, RPL_TOPICWHOTIME(client->getNick(), getName(), (_topic_set_client ? _topic_set_client->getNick() : "server"), _topic_set_date));
}

void Channel::sendTopicMsg(Client *client) const {
	if (_topic.empty()) {
		Server::send(client, RPL_NOTOPIC(client->getNick(), _name));
	}
	else {
		Server::send(client, RPL_TOPIC(client->getNick(), _name, _topic));
		sendTopicWhoMsg(client);
	}
}

void Channel::broadcastTopicMsg(Client *client) {
	if (_topic.empty()) {
		for (std::set<Client *>::iterator it = _users.begin(); it != _users.end(); it++) {
			Server::send(*it, RPL_NOTOPIC(client->getNick(), _name));
		}
	}
	else {
		for (std::set<Client *>::iterator it = _users.begin(); it != _users.end(); it++) {
			Server::send(*it, RPL_TOPIC((*it)->getNick(), _name, _topic));
			if (client) {
				sendTopicWhoMsg(*it);
			}
		}
	}
}

void Channel::setTopic(Client *client, const std::string& new_topic) {
	time_t	t = std::time(NULL);

	_topic = new_topic;
	_topic_set_client = client;
	_topic_set_date = ctime(&t);

	broadcastTopicMsg(client);
}

bool Channel::isUserOp(const std::string &nick) const {
	if (getOpbyNick(nick) != _op.end())
		return true;
	return false;
}
bool Channel::isUserOp(Client *client) {
	std::set<Client *>::iterator it = _op.find(client);
	if (it != _op.end())
		return true;
	return false;
}

void	Channel::kickUser(Client *kicker, const std::string& target, const std::string& comment) {
	if (isUserOnChannel(target)) {
		broadcastMsg(RPL_KICK(kicker->getNick(), kicker->getUser(), kicker->getHost(), getName(), target, comment));
		delUser(target);
	}
	else {
		Server::send(kicker, ERR_USERNOTINCHANNEL(kicker->getNick(), target, getName()));
	}
}


/* ************************************************************************** */

