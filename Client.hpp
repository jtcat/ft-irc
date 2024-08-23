#pragma once

#include <iostream>
#include <string>
#include <set>

class Channel;
#include "Channel.hpp"

class Client {
	public:
		Client(void);
		Client(Client const & src );
		~Client(void);

		Client &		operator=( Client const & rhs );

	private:
		int								_sock_fd;
		std::string 					_nick;
		std::string 					_username;
		int								_autheticated;
		std::set<std::string, Channel *> _channels;
};

std::ostream &			operator<<( std::ostream & o, Client const & i );
