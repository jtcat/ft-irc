#include "Chanel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Chanel::Chanel()
{
}

Chanel::Chanel( const Chanel & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Chanel::~Chanel()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Chanel &				Chanel::operator=( Chanel const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Chanel const & i )
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
