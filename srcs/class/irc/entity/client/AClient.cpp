#include "AClient.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AClient::AClient(IProtocol * protocol, SockStream & socket) : AEntity(), _protocol(protocol), _socket(&socket), _givenPassword("")
{
	this->_protocol = protocol;
	this->_socket = &socket;
}

// AClient::AClient( const AClient & src )
// {
// }


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AClient::~AClient()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

AClient &				AClient::operator=( AClient const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	(void) rhs;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

SockStream&				AClient::getStream( void )
{
	return *this->_socket;
}

std::string				AClient::getPassword( void )
{
	return this->_givenPassword;
}

void					AClient::setPassword( std::string password) 
{
	this->_givenPassword = password;
}

/* ************************************************************************** */