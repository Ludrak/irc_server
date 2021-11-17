# include "ClientInfo.hpp"

ClientInfo::ClientInfo(
	const std::string &real_name,
	const uint mode,
	const std::string &server_token,
	const std::string &server_host
)
:	_mode(mode),
	_realname(real_name),
	_serverToken(server_token),
	_host(server_host),
	_serverOp(false),
	_concurrentChannels(0),
	_concurrentChannelsMax(NB_CLIENT_REGISTRATION_MAX)
{
}

bool						ClientInfo::maxChannelAccessReached( void ) const
{
	return (this->_concurrentChannels == this->_concurrentChannelsMax);
}

uint						ClientInfo::getConcurrentChannels( void ) const
{
	return this->_concurrentChannels;
}

uint						ClientInfo::getConcurrentChannelsMax( void ) const
{
	return this->_concurrentChannelsMax;
}

void						ClientInfo::incrementJoinedChannels( void )
{
	if (this->_concurrentChannels == this->_concurrentChannelsMax)
	{
		Logger::critical("Incrementing number of joined channel at max value");
		return;
	}
	this->_concurrentChannels++;
}

void						ClientInfo::decrementJoinedChannels( void )
{
	if (this->_concurrentChannels == 0)
	{
		Logger::critical("Decrementing number of joined channel at zero value");
		return;
	}
	this->_concurrentChannels--;
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

bool						ClientInfo::isEnable(uint modeMask) const
{
	return (this->_mode & modeMask);	
}

uint						ClientInfo::getMode() const
{
	return (this->_mode);
}

const std::string&			ClientInfo::getRealname( void ) const
{
	return this->_realname;
}

void						ClientInfo::setRealname( const std::string &realname )
{
	this->_realname = realname;
}

const std::string&			ClientInfo::getServerToken( void ) const
{
	return (this->_serverToken);
}

void						ClientInfo::setServerToken( const std::string &token )
{
	this->_serverToken = token;
}

const std::string&			ClientInfo::getHostname( void ) const
{
	return (this->_host);
}

void						ClientInfo::setHostname( const std::string &host )
{
	this->_host = host;
}

bool						ClientInfo::isServerOP( void ) const
{
	return (this->_serverOp > 0);
}

void						ClientInfo::setServerOP( const bool op )
{
	this->_serverOp = op;
}


