#include "IRCServer.hpp"

const uint				IRCServer::value_type = 42;

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
//REVIEW Server name maximum 63 character
// TODO set server token, name & info 
IRCServer::IRCServer(ushort port, const std::string & password, const std::string &host)
: ASockManager(), ANode(host), AEntity(IRCServer::value_type, "token"), ServerInfo("name", "info", "pass", "IRC|amazircd"), _handler(*this), _protocol()
{
	this->_initCommands();
	Logger::debug("IRCServer constructor");
	Logger::info("IRC Server:");
	Logger::info("- host     : " + ntos(host));
	Logger::info("- port     : " + ntos(port));
	Logger::info("- password :" + ntos(password));
	this->listenOn(port, this->_protocol);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/



IRCServer::~IRCServer()
{
	// delete all entities
	for (std::map<std::string, AEntity *>::iterator it = this->_entities.begin(); it != this->_entities.end(); ++it)
	{
		delete (*it).second;
	}
}



/*
** --------------------------------- METHODS ----------------------------------
*/



// TODO REFRACTOR
void			IRCServer::_printServerState( void )
{
	Logger::warning("TODO print server state");
	// Logger::debug("Server : p: " + ntos(this->_pendingConnections.size()) + ", r: " + ntos(this->_ircClients.size()) );
	// Logger::debug("pending:");
	// for (std::list<Client *>::iterator it = this->_pendingConnections.begin(); it != this->_pendingConnections.end(); ++it)
	// 	Logger::debug("   - N: " + (*it)->getNickname() + " / P: " + (*it)->getPassword() + " / R: " + ntos((*it)->isRegistered()) + " / S: " + ntos((*it)->getStream().getSocket()));
	// Logger::debug("registered:");
	// for (std::map<std::string, AEntity *>::iterator it = this->_ircClients.begin(); it != this->_ircClients.end(); ++it)
	// {
	// 	if (it->second->getType() == Channel::value_type_channel)
	// 	{
	// 		Channel *chan = reinterpret_cast<Channel *>(it->second);
	// 		Logger::debug("Channel K:" + it->first + " / N: " + chan->getNickname() + " / P: " + chan->getPassword());
	// 		if (chan->getCreator() == NULL)
	// 			Logger::debug("Channel creator: NULL");
	// 		else
	// 			Logger::debug("Channel creator: " + chan->getCreator()->getNickname());
	// 		Logger::debug("Channel moderated: " + ntos(chan->isEnable(M_MODERATED)) );

	// 	}
	// 	else
	// 	{
	// 		Client * cli = reinterpret_cast<Client *>(it->second);
	// 		Logger::debug("   - K:" + it->first + " / N: " + it->second->getNickname() + " / P: " + cli->getPassword() + " / R: " + ntos(cli->isRegistered()));
	// 	}
	// }
}




bool							IRCServer::connectToNetwork(const std::string & host, ushort port, std::string & password)
{
	// this->_forword_socket = new SockStream(host, port);
	Logger::info("Try connecting to network:"); 
	Logger::info("- host     : " + ntos(host)); 
	Logger::info("- port     : " + ntos(port)); 
	Logger::info("- password : " + ntos(password));
	try
	{
		if (this->connectOn(host, port, this->_protocol) == false)
		{
			Logger::error("Cannot connect to network");
			return false;
		}
	}
	catch (const AClient::ConnectionException &e)
	{
		// connection exception, we are root on that server until another connection succeeds
		Logger::info ("Forward connection failed: running server as root node");
		return false;
	}
	return true;
}




/*
** ----------------------------- Client management --------------------------------
*/



void							IRCServer::_addClient(AEntity &client)
{	
	if (client.getFamily() == CLIENT_ENTITY_FAMILY)
	{
		bool already_exists;

		already_exists = this->_entities.insert(std::make_pair(client.getUID(), &client)).second;
		if (already_exists)
		{
			Logger::critical("double insertion in _entities: trying to add a new client to an already used nickname");
			return;
		}
		already_exists = this->_clients.insert(std::make_pair(client.getUID(), &client)).second;
		if (already_exists)
		{
			Logger::critical("double insertion in _clients: trying to add a new client to an already used nickname");
			return;
		}
		return ;
	}
	Logger::critical("bad entity insertion: trying to add a non-client family entity to _addClient()");
}



void							IRCServer::_addServer(AEntity &server)
{	
	if (server.getFamily() == SERVER_ENTITY_FAMILY)
	{
		bool already_exists;

		already_exists = this->_entities.insert(std::make_pair(server.getUID(), &server)).second;
		if (already_exists)
		{
			Logger::critical("double insertion in _entities: trying to add a new server to an already used nickname");
			return;
		}
		already_exists = this->_servers.insert(std::make_pair(server.getUID(), &server)).second;
		if (already_exists)
		{
			Logger::critical("double insertion in _servers: trying to add a new server to an already used nickname");
			return;
		}
		return ;
	}
	Logger::critical("bad entity insertion: trying to add a non-server family entity to _addServer()");
}



void							IRCServer::_deleteClient(const std::string &nick)
{
	if (this->_entities.count(nick) == 1)
	{
		delete this->_entities[nick];
		this->_entities.erase(nick);
		if (this->_clients.erase(nick) != 1)
		{
			Logger::critical("bad deletion in _clients: trying to delete a new client which does not exist");
			return;
		}
		return ;
	}
	Logger::critical("bad deletion in _entity: trying to delete a new client which does not exist");
}



void							IRCServer::_deleteServer(const std::string &nick)
{
	if (this->_entities.count(nick) == 1)
	{
		delete this->_entities[nick];
		this->_entities.erase(nick);
		if (this->_servers.erase(nick) != 1)
		{
			Logger::critical("bad deletion in _servers: trying to delete a new server which does not exist");
			return;
		}
		return ;
	}
	Logger::critical("bad deletion in _entity: trying to delete a new server which does not exist");
}





void							IRCServer::_sendMessage(AEntity & target, const std::string &message, const AEntity *except)
{
	int type = target.getType() & ~AEntity::value_type & ~NetworkEntity::value_type & ~RelayedEntity::value_type;
	switch (type)
	{
		case Channel::value_type :
		{
			Logger::debug("Sending channel message");
			Package package(this->_protocol, this->_protocol.format(message));
			reinterpret_cast<Channel*>(&target)->broadcastPackage(package, &reinterpret_cast<const Client*>(except)->getStream());
			break;
		}
		case Client::value_type :
		{
			Logger::debug("Sending Client message");
			Package *package = new Package(this->_protocol, this->_protocol.format(message), &reinterpret_cast<Client*>(&target)->getStream());
			this->sendPackage(package, reinterpret_cast<Client*>(&target)->getStream());
			break;
		}	
		case RelayedClient::value_type :
		{
			Logger::debug("Sending RelayedClient message");
			Package *package = new Package(this->_protocol, this->_protocol.format(message), &reinterpret_cast<RelayedClient*>(&target)->getServer().getStream());
			this->sendPackage(package, reinterpret_cast<RelayedClient*>(&target)->getServer().getStream());
			break;
		}
		case Server::value_type :
		{
			Logger::debug("Sending Server message");
			Package *package = new Package(this->_protocol, this->_protocol.format(message), &reinterpret_cast<Server*>(&target)->getStream());
			this->sendPackage(package, reinterpret_cast<Server*>(&target)->getStream());
			break;
		}
		case RelayedServer::value_type :
		{
			Logger::debug("Sending RelayedServer message");
			Package *package = new Package(this->_protocol, this->_protocol.format(message), &reinterpret_cast<RelayedServer*>(&target)->getServer().getStream());
			this->sendPackage(package, reinterpret_cast<RelayedServer*>(&target)->getServer().getStream());
			break;
		}
		case UnRegisteredConnection::value_type :
		{
			Logger::debug("Sending UnregisteredConnection message");
			Package *package = new Package(this->_protocol, this->_protocol.format(message), &reinterpret_cast<UnRegisteredConnection*>(&target)->getStream());
			this->sendPackage(package, reinterpret_cast<UnRegisteredConnection*>(&target)->getStream());
			break;
		}
		default:
			Logger::critical("Unknown entity type to send message: " + message);
	}
}



void							IRCServer::_sendMessage(AEntity & target, const std::stringstream &message, const AEntity *except)
{
	this->_sendMessage(target, message.str(), except);
}




void							IRCServer::_sendMessage(SockStream & target, const std::stringstream &message)
{
	Package *pkg = new Package(this->_protocol, this->_protocol.format(message.str()), &target);
	this->sendPackage(pkg, target);
}




void							IRCServer::_sendMessage(SockStream & target, const std::string &message)
{
	Package *pkg = new Package(this->_protocol, this->_protocol.format(message), &target);
	this->sendPackage(pkg, target);
}



/*
** ----------------------------- Server events ----------------------------------
*/




void						IRCServer::_onClientJoin(SockStream & s)
{
	UnRegisteredConnection *connection(new UnRegisteredConnection(s));
	this->_unregistered_connections.insert(std::make_pair(&s, connection));
	this->_connections.insert(std::make_pair(&s, connection));
	Logger::info("new incomming connection from " + s.getIP());
	this->_printServerState();
}




void							IRCServer::_onClientRecv(SockStream & s, Package & pkg)
{
	NetworkEntity *entity = getEntityByStream(s);
	if (!entity || pkg.getData().empty())
		return ;
	uint ret = this->_handler.handle(*entity, pkg.getData());
	if (ret)
		Logger::error("something bad happened in handler");
}




// TODO REFRACTOR ALL
void							IRCServer::_onClientQuit(SockStream &s)
{
	(void)s;
	//TODO erase client from _connections
	//TODO erase client from _unregistered (if unregistered)
	//TODO erase client from _entities
	/*Client* cli = this->getClientBySockStream(s);
	if (cli == NULL)
	{
		Logger::critical("SockStream without Client associated !!! " + ntos(s.getSocket()));
		return ;
	}
	if (cli->isRegistered())
	{
		Logger::warning("Client " + cli->getNickname() + " disconnected.");
		cli->leaveAllChannels();
		if (cli->getType() == Client::value_type_server)
		{*/
			/* for server SID is used as unique identifier */
			/* disconnect all servers correspondig to that socket since those servers are now netsplitted from the network */
			/*for (std::map<std::string, AEntity *>::iterator it = this->_ircClients.begin(); it != this->_ircClients.end();)
			{
				Client *lost_server = reinterpret_cast<Client*>(it->second);
				if (it->second->getType() == Client::value_type_server && lost_server->getStream().getSocket() == s.getSocket())
				{
					if (lost_server->isRelayed())
						Logger::warning("netsplit occured ! lost connection from server " + lost_server->getSID() + "@" + lost_server->getStream().getIP());
					delete this->_ircClients[lost_server->getSID()];
					this->_ircClients.erase(it++);
				}
				else ++it;
			}
			return ;
		}
		std::string nick = cli->getNickname();
		delete this->_ircClients[nick];
		this->_ircClients.erase(nick);
	}
	else
	{
		Logger::warning("unknown client " + ntos(s.getSocket()) + " disconnected.");
		delete cli;
		this->_pendingConnections.remove(cli);
	}*/
}




/***** Client events *****/




// TODO REFRACTOR AND HANDLE FORWARD
void						IRCServer::_onRecv(SockStream &server, Package &pkg)
{
	(void)server;
	(void)pkg;	
	Logger::warning("TODO HANDLE server received");
}




// TODO REFRACTOR AND HANDLE FORWARD
void				IRCServer::_onConnect ( SockStream &server)
{
	Logger::info("Connecting to forward server");
	this->_unregistered_connections.insert(std::make_pair(&server, new UnRegisteredConnection(server)));
	std::stringstream ss;
	ss << "SERVER " << this->_name << " 0 " << this->getUID() << " :" << this->_info; 
	this->_sendMessage(server, ss);
}




// TODO REFRACTOR AND HANDLE FORWARD
void				IRCServer::_onQuit( SockStream &server)
{
	(void)server;
	Logger::warning("Stop connection to forward server");
	Logger::warning("TODO handle netsplit with forward network");
}




/*
** --------------------------------- ACCESSOR ---------------------------------
*/




NetworkEntity*						IRCServer::getEntityByStream(SockStream & s)
{
	/* get in unregistered list */
	if (this->_unregistered_connections.count(&s) == 1)
		return (this->_unregistered_connections[&s]);
	/* get in global connections */
	if (this->_connections.count(&s) == 1)
		return (this->_connections[&s]);
	Logger::critical("received from SockStream not attached to any Entity");
	return (NULL);
}




const IProtocol&				IRCServer::getProtocol( void ) const
{
	return this->_protocol;
}




UnRegisteredConnection*		IRCServer::getUnRegisteredConnectionByUID(std::string UID)
{
	for (std::map<SockStream*, UnRegisteredConnection *>::iterator it = this->_unregistered_connections.begin(); it != this->_unregistered_connections.end(); ++it)
	{
		if (!it->second->getUID().empty() && it->second->getUID().compare(UID) == SUCCESS)
			return it->second;
	}
	return NULL;
}




/*
** --------------------------------- COMMANDS ---------------------------------
*/




void							IRCServer::_initCommands( void )
{
	this->_handler.addCommand<CommandUser>("USER");
	this->_handler.addCommand<CommandPass>("PASS");
	this->_handler.addCommand<CommandNick>("NICK");
	// this->handle.addComand<CommandServer>("SERVER");
	// this->_handle.addCommand<CommandPrivmsg>("PRIVMSG");
	// this->_handle.addCommand<CommandJoin>("JOIN");
	// this->_handle.addCommand<CommandMode>("MODE");

}




bool							IRCServer::alreadyInUseUID(std::string & uid) const 
{
	if (this->_entities.count(uid) != 0)
		return true;
	for (std::map<SockStream*, UnRegisteredConnection*>::const_iterator it = this->_unregistered_connections.begin(); 
		it != this->_unregistered_connections.end();
		++it)
	{
		if (it->second->getUID().compare(uid) == SUCCESS)
			return true;
	}
	return false;
}




// prefix format -> :<server_uid>[!<user_uid>@<host_uid>]SPACE
std::string							IRCServer::makePrefix(const AEntity *user, const AEntity *host_server)
{
	std::string prefix = ":";
	prefix += this->getUID();

	if (user && host_server 
	&& (user->getType() & Client::value_type || user->getType() & RelayedClient::value_type)
	&& (host_server->getType() & Server::value_type || host_server->getType() & RelayedServer::value_type))
	{
		prefix += "!" + user->getUID() + "@" + host_server->getUID() + " ";
	}
	else
	{
		prefix += " ";
	}
	return (prefix);
}




// prefix format -> :<server_uid>[!<user_uid>@<host_uid>]SPACE
bool								IRCServer::parsePrefix(const std::string &prefix, Server **const sender_server, RelayedClient **const user, RelayedServer **const host_server)
{
	if (prefix.find(":") != 1 || !sender_server)
		return (false);
	/* extended prefix */
	if (prefix.find("!") != std::string::npos && user && host_server)
	{
		/* parse sender */
		std::string token = prefix.substr(1, prefix.find ("!") - 1);
		if (this->_servers.count(token) != 1 || this->_servers[token]->getType() & ~Server::value_type)
		{
			Logger::critical("extended prefix sender server is not a direct server");
			return (false);
		}
		*sender_server = reinterpret_cast<Server*>(this->_servers[token]);

		/* parse sender client nickname */
		token = prefix.substr(prefix.find ("!") + 1, prefix.find("@") - prefix.find ("!") - 1);
		if (this->_clients.count(token) != 1 || this->_clients[token]->getType() & ~RelayedClient::value_type)
		{
			Logger::critical("extended prefix username is not a relayed client");
			return (false);
		}
		*user = reinterpret_cast<RelayedClient*>(this->_clients[token]);

		/* parse host server */
		token = prefix.substr(prefix.find("@") + 1, prefix.size() - prefix.find("!") - 1);
		if (this->_servers.count(token) != 1 || this->_servers[token]->getType() & ~RelayedServer::value_type)
		{
			Logger::critical("extended prefix host is not a relayed server");
			return (false);
		}
		*host_server = reinterpret_cast<RelayedServer*>(this->_servers[token]);
	}
	/* simple prefix */
	else
	{
		*user = NULL;
		*sender_server = NULL;
		std::string token = prefix.substr(1, prefix.size() - 1);
		if (this->_servers.count(token) != 1 || this->_servers[token]->getType() & ~Server::value_type)
		{
			Logger::critical("extended prefix sender server is not a direct server");
			return (false);
		}
		*sender_server = reinterpret_cast<Server*>(this->_servers[token]);
	}
	return (true);
}

/* ************************************************************************** */