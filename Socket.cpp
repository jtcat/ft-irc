#include "Socket.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Socket::Socket()
{
}

Socket::Socket( const Socket & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Socket::~Socket()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Socket &				Socket::operator=( Socket const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Socket const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */