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

bool						ChannelInfo::isEnable(uint modeMask)
{
	return (this->_mode & modeMask);	
}