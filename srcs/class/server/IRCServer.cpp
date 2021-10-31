#include "IRCServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
//REVIEW Server name maximum 63 character
//REVIEW nickname name maximum 9 character

IRCServer::IRCServer() : AServer(this->_protocol), _protocol(IRCProtocol())
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
** --------------------------------- EVENTS ----------------------------------
*/


void							IRCServer::_onClientJoin(SockStream &s)
{
	std::cout << "[IRC] Client " << s.getSocket() << " joined the server !" << std::endl;
	s.setPackageProtocol(this->_protocol);
}

void							IRCServer::_onClientRecv(SockStream &s, const Package pkg)
{
	std::cout << "[IRC]<" << s.getSocket() << "> " << pkg.getData();
}

void							IRCServer::_onClientQuit(SockStream &s)
{
	std::cout << "[IRC] Client " << s.getSocket() << " disconnected." << std::endl;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int								IRCServer::getNetworkSocket( void ) const
{
	return this->_Server_network_socket;
}

/* ************************************************************************** */