#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(const SockStream & socket) : AEntity()
{
	this->_socket = socket.getSocket();

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
	return Client::value_type;
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

void					Client::setRegistered( bool registered)
{
	this->_registered = registered;
}

bool					Client::isRegistered( void )
{
	return this->_registered;
}

SockStream&				Client::getStream( void )
{
	SockStream* s = this->_master->getSocket(this->_socket);
	if (s == NULL)
	{
		Logger::critical("Client try to get invalid socket with socketId.");
		//TODO throw not found
		// throw ;
	}
	return *s;
}

/* ************************************************************************** */