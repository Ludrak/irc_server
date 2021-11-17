#include "ACommand.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ACommand::ACommand() : _sender(NULL), _client(NULL), _clientHost(NULL)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ACommand::~ACommand()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ACommand &				ACommand::operator=( ACommand const & rhs )
{
	if (this != &rhs)
	{
		this->_sender = rhs._sender;
		this->_client = rhs._client;
		this->_clientHost = rhs._clientHost;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

bool				ACommand::hasPermissions(AEntity & executor)
{
	/* By default all permissions is granted*/
	(void) executor;
	return SUCCESS;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


const Server*		ACommand::getSender( void ) const
{
	return this->_sender;
}

const AEntity*		ACommand::getClient( void ) const
{
	return this->_client;
}

const AEntity*		ACommand::getClientHost( void ) const
{
	return this->_clientHost;
}

void				ACommand::setSender(Server * sender)
{
	this->_sender = sender;
}

void				ACommand::setClient(AEntity * client)
{
	this->_client = client;
}

void				ACommand::setClientHost(AEntity * Host)
{
	this->_clientHost = Host;
}


/* ************************************************************************** */