#include "AServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AServer::AServer() : ASockStream()
{
}

AServer::AServer( const AServer & src ) : _clients(src._clients)
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
	(void) rhs;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, AServer const & i )
{
	//o << "Value = " << i.getValue();
	(void ) i;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

Client						*AServer::_acceptConnection(ASockStream &socket_client)
{
	return new Client(socket_client);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */