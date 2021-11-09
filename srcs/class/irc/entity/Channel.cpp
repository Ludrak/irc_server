#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel() : AEntity(), ASockHandler()
{
}

Channel::Channel( const Channel & src ) : _clients(src._clients), _operators(src._operators)
{

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

std::ostream &			operator<<( std::ostream & o, Channel const & i )
{
	o << "Channel = ";
	o << "- id	= " << i.getUID();
	// o << "- usernames	= " << i.getId();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

//TODO remove
void				Channel::broadcastPackage(Package * pkg, const SockStream * except)
{
	(void) pkg;
	(void) except;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

uint					Channel::getType( void ) const
{
	return Channel::value_type;
}

/* ************************************************************************** */