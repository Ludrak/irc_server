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

void						ChannelInfo::setConcurrentClientsMax( const uint new_max )
{
	this->_concurrentClientsMax = new_max;
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

bool						ChannelInfo::hasMode(uint modeMask) const
{
	return (this->_mode & modeMask);	
}

void						ChannelInfo::setMode(uint modeMask, int set)
{
	if (set > 0)
		this->enableMode(modeMask);
	else
		this->disableMode(modeMask);
}

uint						ChannelInfo::getMode(void) const
{
	return (this->_mode);
}

uint						ChannelInfo::getModeBit(const char c) const
{
	switch (c)
	{
		case 'O':
			return (MODE_CHAN_CREATOR);
		case 'o':
			return (MODE_CHAN_OP);
		case 'a':
			return (MODE_CHAN_ANONYMOUS);
		case 'i':
			return (MODE_CHAN_INVITEONLY);
		case 'm':
			return (MODE_CHAN_MODERATED);
		case 'n':
			return (MODE_CHAN_LOCALONLY);
		case 'q':
			return (MODE_CHAN_QUIET);
		case 'p':
			return (MODE_CHAN_PRIVATE);
		case 's':
			return (MODE_CHAN_SECRET);
		case 'r':
			return (MODE_CHAN_SERVREOP);
		case 't':
			return (MODE_CHAN_TOPICOPONLY);
		case 'k':
			return (MODE_CHAN_HASKEY);
		case 'l':
			return (MODE_CHAN_HASLIMIT);
		case 'b':
			return (MODE_CHAN_HASBANMASK);
		case 'e':
			return (MODE_CHAN_HASBANEXCEPTIONS);
		case 'I':
			return (MODE_CHAN_HASINVITATIONSMASK);
		default:
			return (0);
	}
	return (0);
}

std::string					ChannelInfo::getModeString( void ) const
{
	std::string mode_str;
	if (this->_mode & MODE_CHAN_CREATOR)
		mode_str += "O ";
	if (this->_mode & MODE_CHAN_OP)
		mode_str += "o ";
	if (this->_mode & MODE_CHAN_ANONYMOUS)
		mode_str += "a ";
	if (this->_mode & MODE_CHAN_INVITEONLY)
		mode_str += "i ";
	if (this->_mode & MODE_CHAN_MODERATED)
		mode_str += "m ";
	if (this->_mode & MODE_CHAN_LOCALONLY)
		mode_str += "n ";
	if (this->_mode & MODE_CHAN_QUIET)
		mode_str += "q ";
	if (this->_mode & MODE_CHAN_PRIVATE)
		mode_str += "p ";
	if (this->_mode & MODE_CHAN_SECRET)
		mode_str += "s ";
	if (this->_mode & MODE_CHAN_SERVREOP)
		mode_str += "r ";
	if (this->_mode & MODE_CHAN_TOPICOPONLY)
		mode_str += "t ";
	
	/* parametized modes */
	if (this->_mode & MODE_CHAN_HASKEY)
		mode_str += "k ";
	if (this->_mode & MODE_CHAN_HASLIMIT)
		mode_str += "l ";
	if (this->_mode & MODE_CHAN_HASBANMASK)
		mode_str += "b ";
	if (this->_mode & MODE_CHAN_HASBANEXCEPTIONS)
		mode_str += "e ";
	if (this->_mode & MODE_CHAN_HASINVITATIONSMASK)
		mode_str += "I ";
	return (mode_str);
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