#include "IRCServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
//REVIEW Server name maximum 63 character
//REVIEW nickname name maximum 9 character

IRCServer::IRCServer() : AServer()
{
	std::cout << "IRCServer constructor" << std::endl;
}

IRCServer::IRCServer( const IRCServer & src ) : AServer(src)
{
	this->_Server_network_socket = src._Server_network_socket;
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
	if ( this != &rhs )
	{
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, IRCServer const & i )
{
	o << "IRCServer = " ;
	o << "_servver_network_socket = " << i.getNetworkSocket();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

bool							IRCServer::_init_server( void )
{
	std::cout << "IRCServer initialisation" << std::endl;
	return true;
}

Client   				       *IRCServer::_acceptConnection(SockStream &socket_client)
{
	std::cout << "_accept a connection" << std::endl;
	return new Client(socket_client);
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int								IRCServer::getNetworkSocket( void ) const
{
	return this->_Server_network_socket;
}

/* ************************************************************************** */