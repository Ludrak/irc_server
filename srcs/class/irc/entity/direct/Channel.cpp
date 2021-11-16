#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel(const std::string & channelName) : AEntity(Channel::value_type, channelName), ASockHandler(), _creator(NULL)
{
	this->_uid = channelName;
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
	if (this->isRegistered(client) == true)
		return ERR_ALREADYREGISTRED;
	else if (client.incRegistration() == false)
		return ERR_TOOMANYCHANNELS;
	else if (this->incRegistration() == false)
		return ERR_CHANNELISFULL;
	this->_clients.push_back(&client);
	this->addSocket(client.getStream());
	Logger::info("<" + client.getUID() + "> join channel <" + this->getUID() + ">");
	//TODO send here the message to everyone saying someone is added to channel
	//TODO this involve adding a protocol to Channel (this will be necessary for TLS ? ) 
	return SUCCESS;
}

uint					Channel::removeClient(Client & client)
{
	if (this->isRegistered(client) == false)
		return ERR_NOTONCHANNEL;
	if (this->decRegistration() == false)
		Logger::critical("Presence in channel should already be tested here");
	if (client.decRegistration())
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

Client*					Channel::getCreator( void )
{
	return this->_creator;
}			

void					Channel::setCreator(Client & client)
{
	this->_creator = &client;
}


/* ************************************************************************** */