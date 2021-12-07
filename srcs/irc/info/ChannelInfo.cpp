# include "ChannelInfo.hpp"


bool						ChannelInfo::isFull( void ) const
{
	return (this->_concurrentClients == this->_concurrentClientsMax);
}

uint						ChannelInfo::getConcurrentClients( void ) const
{
	return this->_concurrentClients;
}

uint						ChannelInfo::getConcurrentClientsMax( void ) const
{
	return this->_concurrentClientsMax;
}

void						ChannelInfo::toogleMode(uint modeMask)
{
	this->_mode ^= modeMask;
}

void						ChannelInfo::enableMode(uint modeMask)
{
	this->_mode |= modeMask;
}

void						ChannelInfo::disableMode(uint modeMask)
{
	this->_mode &= ~modeMask;
}

bool						ChannelInfo::isEnable(uint modeMask) const
{
	return (this->_mode & modeMask);	
}

bool						ChannelInfo::incrementJoinedClients( void )
{
	if (this->_concurrentClients == this->_concurrentClientsMax)
	{
		Logger::critical("Incrementing number of joined channel at max value");
		return (false);
	}
	this->_concurrentClients++;
	return (true);
}

bool						ChannelInfo::decrementJoinedClients( void )
{
	if (this->_concurrentClients == 0)
	{
		Logger::critical("Decrementing number of joined channel at zero value");
		return (false);
	}
	this->_concurrentClients--;
	return (true);
}