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

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

IRCServer::~IRCServer()
{
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