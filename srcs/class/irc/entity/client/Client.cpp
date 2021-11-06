#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(IRCServer & master, SockStream & socket) : AIrcClient(master.getProtocol(), socket)
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

uint					Client::getId( void ) const
{
	return Client::value_type;
}


void				Client::setUsername(std::string user)
{
	this->_username = user;
}

void				Client::setDomaineName(std::string domaine)
{
	this->_domaine = domaine;
}

void				Client::setServername(std::string servername)
{
	this->_servername = servername;
}

void				Client::setRealname(std::string realname)
{
	this->_realname = realname;
}

std::string			Client::getUsername( void )
{
	return this->_username;
}

std::string			Client::getDomaineName( void )
{
	return this->_domaine;
}

std::string			Client::getServername( void )
{
	return this->_servername;
}

std::string			Client::getRealname( void )
{
	return this->_realname;
}


/* ************************************************************************** */