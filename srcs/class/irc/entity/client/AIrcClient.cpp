#include "AIrcClient.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AIrcClient::AIrcClient(SockStream & socket) : AEntity(), _socket(&socket), _givenPassword(""), _nickname(""), _registered(false)
{
	this->_socket = &socket;
}

// AIrcClient::AIrcClient( const AIrcClient & src )
// {
// }


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AIrcClient::~AIrcClient()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

AIrcClient &				AIrcClient::operator=( AIrcClient const & rhs )
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


bool				AIrcClient::isRegistered( void )
{
	return this->_registered;
}

SockStream&				AIrcClient::getStream( void )
{
	return *this->_socket;
}

std::string				AIrcClient::getPassword( void )
{
	return this->_givenPassword;
}

std::string				AIrcClient::getNickname(void)
{
	return this->_nickname;
}

void				AIrcClient::setRegistered( bool registered)
{
	this->_registered = registered;
}

void					AIrcClient::setPassword( std::string password) 
{
	this->_givenPassword = password;
}

void					AIrcClient::setNickname(std::string nick)
{
	this->_nickname = nick;
}
/* ************************************************************************** */