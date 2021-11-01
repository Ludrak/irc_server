#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(IProtocol & protocol) : SockStream(protocol), AEntity(), _protocol(&protocol)
{
}

Client::Client(SockStream &master) :  SockStream(NULL, master.getSocket(), master.getSockaddr(), *master.getProtocol()), AEntity(), _protocol(master.getProtocol())
{
	
}

Client::Client(const AServer *server, int socket, const sockaddr_in &addr, IProtocol & protocol) : SockStream(server, socket, addr, protocol), _protocol(&protocol)
{

}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client &				Client::operator=( Client const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
		(void) rhs;
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Client const & i )
{
	//o << "Value = " << i.getValue();
	(void) i;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */