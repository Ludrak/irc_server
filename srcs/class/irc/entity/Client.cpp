#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(SockStream & socket) : AEntity(), _type(Client::value_type_unknown)
{
	this->_socket = &socket;

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

uint					Client::getType( void ) const
{
	return (this->_type);
}

std::string				Client::getUsername( void )
{
	return this->_username;
}

void					Client::setUsername(std::string user)
{
	this->_username = user;
}

std::string				Client::getDomaineName( void )
{
	return this->_domaine;
}

void					Client::setDomaineName(std::string domaine)
{
	this->_domaine = domaine;
}

std::string				Client::getServername( void )
{
	return this->_servername;
}

void					Client::setServername(std::string servername)
{
	this->_servername = servername;
}

std::string				Client::getRealname( void )
{
	return this->_realname;
}

void					Client::setRealname(std::string realName)
{
	this->_realname = realName;
}


void					Client::setRegisteredAsClient( bool registered)
{
	this->_registered = registered;
	this->_type = Client::value_type_client;
}

void					Client::setRegisteredAsServer( bool registered)
{
	this->_registered = registered;
	this->_type = Client::value_type_server;
}

bool					Client::isRegistered( void )
{
	return this->_registered;
}

SockStream&				Client::getStream( void )
{
	if (this->_socket == NULL)
	{
		Logger::critical("Client try to get invalid socket with socketId.");
		//TODO throw not found
		// throw ;
	}
	return *this->_socket;
}

/* ************************************************************************** */