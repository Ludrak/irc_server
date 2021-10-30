#include "IRCServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

IRCServer::IRCServer()
{
}

IRCServer::IRCServer( const IRCServer & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

IRCServer::~IRCServer()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

IRCServer &				IRCServer::operator=( IRCServer const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, IRCServer const & i )
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