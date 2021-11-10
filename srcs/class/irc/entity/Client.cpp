#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(SockStream & socket) : AEntity(NB_CLIENT_REGISTRATION_MAX)
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

uint					Client::joinChannel( Channel & chan )
{
	uint ret = chan.addClient(*this);
	if (ret != SUCCESS)
		return ret;
	Logger::debug("Adding <" + chan.getNickname() + "> to <" + this->getNickname() + "> channel list");
	this->_channels.push_back(&chan);
	return SUCCESS;
}

uint					Client::leaveChannel( Channel & chan )
{
	uint ret = chan.removeClient(*this);
	if (ret != SUCCESS)
		return ret;
	Logger::debug("Removing <" + chan.getNickname() + "> to <" + this->getNickname() + "> channel list");
	this->_channels.remove(&chan);
	return SUCCESS;
}

		
uint					Client::leaveAllChannels( void )
{
	Logger::debug("client " + this->getNickname() + " leaving all his channels.");
	for (std::list<Channel *>::iterator it = this->_channels.begin() ; it != this->_channels.end(); ++it)
	{
		(*it)->removeClient(*this);
	}
	this->_channels.clear();
	Logger::info("<" + this->getNickname() + "> had leave all his channels.");
	return SUCCESS;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

uint					Client::getType( void ) const
{
	if (this->_socket->getType() == REMOTE_CLIENT)
		return Client::value_type_client;
	return Client::value_type_server;
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
	if (this->_socket == NULL)
	{
		Logger::critical("Client try to get invalid socket with socketId.");
		//TODO throw not found
		// throw ;
	}
	return *this->_socket;
}

/* ************************************************************************** */