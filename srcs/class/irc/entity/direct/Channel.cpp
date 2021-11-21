#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel(const std::string & channelName) : AEntity(Channel::value_type, channelName), ASockHandler(), _creator(NULL)
{
	this->_uid = channelName;
	this->_concurrentClients = 0;
	this->_concurrentClientsMax = 10;
}

Channel::Channel( const Channel & src ) : AEntity(Channel::value_type, src.getUID()), ASockHandler(), _clients(src._clients), _operators(src._operators)
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

uint				Channel::addClient(Client & client)
{
	// TODO send right reply / inc /dec registration
	//TODO change incRegistration for being blocked only when mode `l` is actived
	if (this->isRegistered(client) == true)
		return 2;//ERR_ALREADYREGISTRED;
	else if (this->incrementJoinedClients() == false)
		return 4;//ERR_CHANNELISFULL;
	this->_clients.push_back(&client);
	this->addSocket(client.getStream());
	Logger::info("<" + client.getUID() + "> join channel <" + this->getUID() + ">");
	//TODO send here the message to everyone saying someone is added to channel
	//TODO this involve adding a protocol to Channel (this will be necessary for TLS ? ) 
	return SUCCESS;
}

uint					Channel::removeClient(Client & client)
{
	//TODO send reply & dec / inc registration
	if (this->isRegistered(client) == false)
		return 1;//ERR_NOTONCHANNEL;
	//if (this->decRegistration() == false)
		Logger::critical("Presence in channel should already be tested here");
	//if (client.decRegistration())
		Logger::critical("client registration should already be tested here");
	this->_clients.remove(&client);
	this->delSocket(client.getStream());
	if (this->_creator == &client)
		this->_creator = NULL;
	Logger::info("<" + client.getUID() + "> leave channel <" + this->getUID() + ">");
	return SUCCESS;
}

void            		Channel::delSocket(const SockStream &sock)
{
	this->_sockets.erase(sock.getSocket());
}


void						Channel::broadcastPackage(Package & pkg, const SockStream * except)
{
	for (std::map<ushort, SockStream*>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); ++it)
	{
		if (it->second != except)
		{
			Package* new_pkg = new Package(pkg);
			new_pkg->setRecipient(it->second);
			this->sendPackage(new_pkg, *it->second);
		}
	}
}

bool					Channel::isRegistered(Client & client)
{
	for (std::list<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
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

std::list<Client *>::iterator		Channel::clientBegin( void )
{
	return this->_clients.begin();
}

std::list<Client *>::const_iterator	Channel::clientBegin( void ) const
{
	return this->_clients.begin();
}

std::list<Client *>::iterator		Channel::clientEnd( void )
{
	return this->_clients.end();
}

std::list<Client *>::const_iterator	Channel::clientEnd( void ) const
{
	return this->_clients.end();
}

std::list<Client *>::iterator		Channel::operatorBegin( void )
{
	return this->_operators.begin();
}

std::list<Client *>::const_iterator	Channel::operatorBegin( void ) const
{
	return this->_operators.begin();
}

std::list<Client *>::iterator		Channel::operatorEnd( void )
{
	return this->_operators.end();
}

std::list<Client *>::const_iterator	Channel::operatorEnd( void ) const
{
	return this->_operators.end();
}

/* ************************************************************************** */