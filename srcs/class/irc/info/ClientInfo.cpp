# include "ClientInfo.hpp"

bool						ClientInfo::isFull( void ) const
{
	return (this->_concurrentClients == this->_concurrentClientsMax);
}

uint						ClientInfo::getConcurrentClients( void ) const
{
	return this->_concurrentClients;
}

uint						ClientInfo::getConcurrentClientsMax( void ) const
{
	return this->_concurrentClientsMax;
}

/* mode */
void						ClientInfo::toogleMode(uint modeMask)
{
	this->_mode ^= modeMask;
}

void						ClientInfo::enableMode(uint modeMask)
{
	this->_mode |= modeMask;
}

void						ClientInfo::disableMode(uint modeMask)
{
	this->_mode &= ~modeMask;
}

bool						ClientInfo::isEnable(uint modeMask)
{
	return (this->_mode & modeMask);	
}

const std::string&			ClientInfo::getRealname( void ) const
{
	return this->_realname;
}

const std::string&			ClientInfo::getServerToken( void ) const
{
	return this->_serverToken;
}

const std::string&			ClientInfo::getHostname( void ) const
{
	return this->_host;
}

bool						ClientInfo::isPrivilegied( void ) const
{
	return (this->_privilege > 0);
}

