#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
}

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

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */

