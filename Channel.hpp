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
		Channel(Channel const & src );
		~Channel(void);

		Channel&	operator=(Channel const & rhs );

	private:
		std::set<std::string, Client *> _users;
		std::set<std::string, Client *> _operators;
};

std::ostream &			operator<<( std::ostream & o, Channel const & i );
