#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel(const std::string & channelName) : AEntity(Channel::value_type, channelName), _creator(NULL), _topic("")
{
	this->_uid = channelName;
	this->_concurrentClients = 0;
	this->_concurrentClientsMax = 10;
}

Channel::Channel( const Channel & src ) : AEntity(Channel::value_type, src.getUID()), _clients(src._clients), _operators(src._operators)
{

}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Channel &				Channel::operator=( Channel const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	(void)rhs;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Channel const & i )
{
	o << "Channel = ";
	o << "- id	= " << i.getUID();
	// o << "- usernames	= " << i.getId();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

uint				Channel::addClient(AEntity & client)
{
	//IMPROVEMENT change incRegistration for being blocked only when mode `l` is actived
	if (!(client.getType() & (Client::value_type | RelayedClient::value_type)))
		return (1);
	if (this->isRegistered(client) == true)
		return 462;//ERR_ALREADYREGISTRED;
	else if (!this->incrementJoinedClients())
		return 471;//ERR_CHANNELISFULL;
	this->_clients.push_front(&client);
	Logger::info("<" + client.getUID() + "> join channel <" + this->getUID() + ">");
	return SUCCESS;
}

uint					Channel::removeClient(AEntity & client)
{
	if (client.getFamily() != CLIENT_ENTITY_FAMILY)
	{
		Logger::critical ("removing non client family entity from channel");
		return (1);
	}
	if (this->isRegistered(client) == false)
		return 442;//ERR_NOTONCHANNEL;
	if (!this->decrementJoinedClients())
	{
		Logger::critical ("try to decrement empty channel");
		return (2);
	}
	this->_clients.remove(&client);
	if (this->_creator == &client)
		this->_creator = NULL;
	Logger::info("<" + client.getUID() + "> leave channel <" + this->getUID() + ">");
	return SUCCESS;
}

void					Channel::broadcastPackage(Package & pkg, const std::string &uid)
{
	for (std::list<AEntity*>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (!uid.empty() && (*it)->getUID() == uid)
			continue;

		if ((*it)->getType() & Client::value_type)
		{
			Client &client = static_cast<Client &>(**it);
			Logger::debug("Channel send to Client: " + client.getUID());
			Package* new_pkg = new Package(pkg);
			new_pkg->setRecipient(&client.getStream());
			client.getServerReference().sendPackage(new_pkg, client.getStream());
		}
		/*else if ((*it)->getType() & RelayedClient::value_type)
		{
			RelayedClient &client = static_cast<RelayedClient &>(*(*it));
			Logger::debug("Channel send to relayedClient" + client.getUID());
			Package* new_pkg = new Package(pkg);
			new_pkg->setRecipient(&client.getServer().getStream());
			client.getServerReference().sendPackage(new_pkg, client.getServer().getStream());
		}*/
	}
}

bool					Channel::isRegistered(AEntity & client)
{
	for (std::list<AEntity *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if ((*it)->getUID() == client.getUID())
			return true;
	}
	return false;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const AEntity*						Channel::getCreator( void ) const
{
	return this->_creator;
}			

void								Channel::setCreator(const AEntity & client)
{
	this->_creator = &client;
}

std::list<AEntity *>::iterator		Channel::clientBegin( void )
{
	return this->_clients.begin();
}

std::list<AEntity *>::const_iterator	Channel::clientBegin( void ) const
{
	return this->_clients.cbegin();
}

std::list<AEntity *>::iterator		Channel::clientEnd( void )
{
	return this->_clients.end();
}

std::list<AEntity *>::const_iterator	Channel::clientEnd( void ) const
{
	return this->_clients.cend();
}

std::list<AEntity *>::iterator		Channel::operatorBegin( void )
{
	return this->_operators.begin();
}

std::list<AEntity *>::const_iterator	Channel::operatorBegin( void ) const
{
	return this->_operators.cbegin();
}

std::list<AEntity *>::iterator		Channel::operatorEnd( void )
{
	return this->_operators.end();
}

std::list<AEntity *>::const_iterator	Channel::operatorEnd( void ) const
{
	return this->_operators.cend();
}

const std::string					&Channel::getTopic(void) const
{
	return (this->_topic);
}
void								Channel::setTopic(const std::string &new_topic)
{
	this->_topic = new_topic;
}

/* ************************************************************************** */