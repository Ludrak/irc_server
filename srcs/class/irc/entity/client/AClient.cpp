#include "AClient.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AClient::AClient(IProtocol * protocol, SockStream & socket) : AEntity(), _protocol(protocol), _socket(&socket), _givenPassword(""), _nickname(""), _registered(false)
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


bool				AClient::isRegistered( void )
{
	return this->_registered;
}

SockStream&				AClient::getStream( void )
{
	return *this->_socket;
}

std::string				AClient::getPassword( void )
{
	return this->_givenPassword;
}

std::string				AClient::getNickname(void)
{
	return this->_nickname;
}

void				AClient::setRegistered( bool registered)
{
	this->_registered = registered;
}

void					AClient::setPassword( std::string password) 
{
	this->_givenPassword = password;
}

void					AClient::setNickname(std::string nick)
{
	this->_nickname = nick;
}
/* ************************************************************************** */