# include "ClientInfo.hpp"

ClientInfo::ClientInfo(
	IRCServer	&server_reference,
	const std::string &real_name,
	const uint mode,
	const std::string &server_token,
	const std::string &server_host
)
:	_serverReference(server_reference),
	_mode(mode),
	_realname(real_name),
	_serverToken(server_token),
	_host(server_host),
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

bool						ClientInfo::incrementJoinedChannels( void )
{
	if (this->_concurrentChannels == this->_concurrentChannelsMax)
	{
		Logger::critical("Incrementing number of joined channel at max value");
		return false;
	}
	this->_concurrentChannels++;
	return true;
}

bool						ClientInfo::decrementJoinedChannels( void )
{
	if (this->_concurrentChannels == 0)
	{
		Logger::critical("Decrementing number of joined channel at zero value");
		return false;
	}
	this->_concurrentChannels--;
	return true;
}

/* mode */
void						ClientInfo::setMode(uint modeMask, int set)
{
	if (set > 0)
		this->enableMode(modeMask);
	else
		this->disableMode(modeMask);
}

void						ClientInfo::toggleMode(uint modeMask)
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

bool						ClientInfo::hasMode(uint modeMask) const
{
	return (this->_mode & modeMask);	
}

uint						ClientInfo::getModeBit(const char c) const
{
	switch (c)
	{
		case 'i':
			return (MODE_INVISIBLE);
		case 's':
			return (MODE_RECVSERVNOTICE);
		case 'w':
			return (MODE_WALLOPS);
		case 'o':
			return (MODE_OPERATOR);
		case 'a':
			return (MODE_AWAY);
		case 'r':
			return (MODE_RESTRICTED);
		default:
			return (0);
	}
	return (0);
}

uint						ClientInfo::getMode() const
{
	return (this->_mode);
}

std::string					ClientInfo::getModeString( void )
{
	std::string mode_str;
	if (this->_mode & MODE_INVISIBLE)
		mode_str += "i ";
	if (this->_mode & MODE_RECVSERVNOTICE)
		mode_str += "s ";
	if (this->_mode & MODE_WALLOPS)
		mode_str += "w ";
	if (this->_mode & MODE_OPERATOR)
		mode_str += "o ";
	if (this->_mode & MODE_AWAY)
		mode_str += "a ";
	if (this->_mode & MODE_RESTRICTED)
		mode_str += "r ";
	return mode_str;
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
	return (this->_mode & MODE_OPERATOR);
}

IRCServer					&ClientInfo::getServerReference(void)
{
	return (this->_serverReference);
}


const std::list<Channel*>	&ClientInfo::getChannels() const
{
    return (this->_channels);
}


int 						ClientInfo::joinChannel(Channel &channel)
{
    if (!this->incrementJoinedChannels())
        return (405);
    this->_channels.push_front(&channel);
	return SUCCESS;
}




void                        ClientInfo::leaveAllChannels(const std::string &info)
{
    for (std::list<Channel*>::iterator it = this->_channels.begin(); it != this->_channels.end(); )
    {
		this->getServerReference()._sendAllServers(this->getPrefix() + "PART " + (*it)->getUID() + " :" + info);
        this->leaveChannel(**(it++), info);
    }
    this->_concurrentChannels = 0;
    this->_channels = std::list<Channel*>();
}




void                        ClientInfo::leaveChannel(Channel &channel, const std::string &info)
{
    if (!this->decrementJoinedChannels())
    {
        Logger::critical("Client leaving unjoined channel");
        return;
    }

	for (std::list<AEntity*>::iterator it = channel.clientBegin(); it != channel.clientEnd(); ++it)
	{
		if ((*it)->getType() & NetworkEntity::value_type)
		{
			this->getServerReference()._sendMessage(**it, this->getPrefix() + "PART " + channel.getUID() + " :" + info);
		}
	}
	int err = 0;
	Client *c = dynamic_cast<Client*>(this);
	if (c)
	{
		err = channel.removeClient(*c);
	}
	else
	{
		RelayedClient *rc = dynamic_cast<RelayedClient*>(this);
		if (rc)
			err = channel.removeClient(*rc);
	}
	this->_channels.remove(&channel);

	if (channel.getConcurrentClients() == 0)
	{
		Logger::info("No clients left on " + channel.getUID() + ", deleting it from server");
		this->getServerReference()._channels.erase(channel.getUID());
		this->getServerReference()._entities.erase(channel.getUID());
		delete &channel;
	}
}

