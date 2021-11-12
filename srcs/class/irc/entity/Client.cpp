#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(SockStream & socket) : AEntity(NB_CLIENT_REGISTRATION_MAX), _type(Client::value_type_unknown), _isRelayed(false)
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
	return (this->_type);
}

const std::string		&Client::getUsername( void ) const
{
	return this->_username;
}

void					Client::setUsername(std::string user)
{
	this->_username = user;
}

const std::string		&Client::getDomaineName( void ) const
{
	return this->_domaine;
}

void					Client::setDomaineName(std::string domaine)
{
	this->_domaine = domaine;
}

const std::string		&Client::getServername( void ) const
{
	return this->_servername;
}

void					Client::setServername(std::string servername)
{
	this->_servername = servername;
}

const std::string		&Client::getRealname( void ) const
{
	return this->_realname;
}

void					Client::setRealname(std::string realName)
{
	this->_realname = realName;
}

uint					Client::getHopCount() const
{
	return (this->_hopcount);
}
void					Client::setHopCount( uint hop )
{
	this->_hopcount = hop;
}

const std::string		&Client::getSID() const
{
	return (this->_sid);
}
void					Client::setSID( const std::string & sid )
{
	this->_sid = sid;
}

const std::string				&Client::getVersion( void )
{
	return this->_version;
}

void					Client::setVersion(std::string version)
{
	Logger::debug(this->getNickname() + " set version: " + version);
	this->_version = version;
}

const std::string		&Client::getFlags( void )
{
	return this->_flags;
}

void					Client::setFlags(std::string flags)
{
	Logger::debug(this->getNickname() + " set flags: " + flags);
	this->_flags = flags;
}

void					Client::setRegistered( bool registered)
{
	this->_registered = registered;
}

const std::string		&Client::getServerDescription() const
{
	return (this->_serverinfo);
}
void					Client::setServerDescription(const std::string &desc)
{
	this->_serverinfo = desc;
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

bool					Client::isRelayed() const
{
	return (this->_isRelayed);
}

void					Client::setRelayed( bool relayed )
{
	this->_isRelayed = relayed;
}


SockStream&				Client::getStream( void ) const
{
	if (this->_socket == NULL)
	{
		Logger::critical("Client try to get invalid socket with socketId.");
		//TODO throw not found
		// throw ;
	}
	return *this->_socket;
}
