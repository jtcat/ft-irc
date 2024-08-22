#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
#include <set>
class Channel;
class Client
{

	public:
		Client();
		Client( Client const & src );
		~Client();

		Client &		operator=( Client const & rhs );
	private:
		int			_sock_fd;
		std::string _nick;
		std::string _username;
		int			_autheticated;
		std::set<std::string, Channel *> _channels;
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */
