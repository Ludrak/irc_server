#include "AServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AServer::AServer()
{
}

AServer::AServer( const AServer & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AServer::~AServer()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

AServer &				AServer::operator=( AServer const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, AServer const & i )
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