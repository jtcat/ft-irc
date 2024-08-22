#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <set>
class Client;

class Channel
{
	public:

		Channel();
		Channel( Channel const & src );
		~Channel();

		Channel &		operator=( Channel const & rhs );

	private:
		std::set<std::string, Client *> _users;
		std::set<std::string, Client *> _operators;
};

std::ostream &			operator<<( std::ostream & o, Channel const & i );

#endif /* ********************************************************** Channel_H */
