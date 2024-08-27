#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
}
Channel::Channel(const std::string &name, Client *op) : _name(name), _user_limit(-1) , _invite_Only_flag(0) {
	_op.insert(op);
};

Channel::Channel( const Channel & src )
{
	(void)src;
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
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	(void)rhs;
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
bool Channel::checkIfUserInvited(Client *client) {
	if (_invites.find(client) != _invites.end())
		return true;
	return false;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const std::string &Channel::getName() const {
	return _name;
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

/* ************************************************************************** */

