#include "IRCServer.hpp"

//REVIEW is it a good idea?
// const uint				IRCServer::value_type = Server::value_type | Client::value_type;
const uint				IRCServer::value_type = Server::value_type;

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
//REVIEW Server name maximum 63 character
// TODO set server token, name & info 
IRCServer::IRCServer(ushort port, const std::string & password, const std::string &host, const std::string &ssl_cert_path, const std::string &ssl_key_path, const ushort tls_port)
:	ASockManager(ssl_cert_path, ssl_key_path),
	ANode(host),
	AEntity(IRCServer::value_type, "token"),
	ServerInfo("name", ntos(IRC_CURRENT_VERSION), "default hostname", "IRC|amazircd"),
	_handler(*this),
	_protocol(),
	_forwardPassword(""),
	_creationTime(std::time(NULL)),
	_operName("becomeOper"),
	_operPassword("becomeOper"),
	_shortMotdEnabled(true),
	_debugLevel(0)
{
	this->_initCommands();
	Logger::debug("IRCServer constructor");
	Logger::info("IRC Server:");
	Logger::info("- host     : " + ntos(host));
	Logger::info("- port     : " + ntos(port));
	Logger::info("- password : " + ntos(password));
	this->setPassword(password);

	this->listenOn(port, this->_protocol);

	/* if key and cert are specified, listen on tls port */
	if (!ssl_cert_path.empty() && !ssl_key_path.empty())
		this->listenOn(tls_port, this->_protocol, true);
	
	for (std::map<ushort, SockStream *>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); ++it)
	{
		if (it->second->getType() == SERVER)
		{
			this->ServerInfo::_host = it->second->getHost();
			break ;
		}
	}
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/



IRCServer::~IRCServer()
{
	// delete all entities
	for (std::map<std::string, AEntity *>::iterator it = this->_entities.begin();
		it != this->_entities.end();
		++it)
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
	Logger::debug("TODO finish print server state");
	Logger::debug("--- Channels ---");
	for (std::map<std::string, Channel *>::const_iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
	{
		Logger::debug("-- " + it->second->getUID() + " " + ntos(it->second->getConcurrentClients()) + "/" + ntos(it->second->getConcurrentClientsMax()));
		if (it->second->getCreator())
			Logger::debug("Creator: " + it->second->getCreator()->getUID());
		else
			Logger::debug("Creator: none");
		for (std::list<Client *>::iterator itc = it->second->clientBegin(); itc != it->second->clientEnd(); ++itc)
		{
			Logger::debug("\t- " + (*itc)->getUID());
		}
	}
	Logger::debug("--- servers ---");
	for (std::map<std::string, AEntity *>::const_iterator it = this->_servers.begin(); it != this->_servers.end(); ++it)
	{
		if (it->second->getType() & (Server::value_type | Server::value_type_forward))
		{
			Server *server = reinterpret_cast<Server*>(it->second);
			Logger::debug("-- " + server->getUID() + 
				"\n - name: " + server->getName() +
				"\n - host: " + server->getHostname() +
				"\n - flags: " + server->getFlags() +
				"\n - info: " + server->getInfo());
		}
		else if (it->second->getType() & RelayedServer::value_type)
		{
			RelayedServer* server = reinterpret_cast<RelayedServer*>(it->second);
			Logger::debug("-- " + server->getUID() + 
				"\n - name: " + server->getName() +
				"\n - hop: " + ntos(server->getHopCount()) +
				"\n - host: " + server->getHostname() +
				"\n - relay: " + server->getServer().getUID() +
				"\n - info: " + server->getInfo());
		}
		else
		{
			Logger::error("Invalid server list member type");
		}


	}
}




bool							IRCServer::connectToNetwork(const std::string & host, ushort port, std::string & password, const bool useTLS)
{
	Logger::info("Try connecting to network:"); 
	Logger::info("- host     : " + host); 
	Logger::info("- port     : " + ntos(port)); 
	Logger::info("- password : " + password);
	this->_forwardPassword = password;
	try
	{
		if (this->connectOn(host, port, this->_protocol, useTLS) == false)
		{
			Logger::error("Cannot connect to network");
			return false;
		}
	}
	catch (const AClient::ConnectionException &e)
	{
		// connection exception, we are root on that server until another connection succeeds
		Logger::warning ("Forward connection failed: running server as root node");
		return false;
	}
	return true;
}




/*
** ----------------------------- Client management --------------------------------
*/



void							IRCServer::_addClient(AEntity &client, UnRegisteredConnection * executor)
{	
	if (client.getFamily() != CLIENT_ENTITY_FAMILY)
	{
		Logger::critical("bad entity insertion: trying to add a non-client family entity to _addClient()");
		return ;
	}
	if (executor != NULL) // RELAYED
	{
		this->_unregistered_connections.erase(&executor->getStream());
		this->_connections.erase(&executor->getStream());
		delete executor;
	}
	if (!this->_entities.insert(std::make_pair(client.getUID(), &client)).second)
		Logger::critical("double insertion in _entities: trying to add a new client to an already used nickname");
	else if (!this->_clients.insert(std::make_pair(client.getUID(), &client)).second)
		Logger::critical("double insertion in _clients: trying to add a new client to an already used nickname");// TODO ~ error
	else if (!(client.getType() & RelayedEntity::value_type) && !this->_connections.insert(std::make_pair(&static_cast<Client &>(client).getStream(), reinterpret_cast<NetworkEntity *>(&client))).second)
		Logger::critical("double insertion in _connections: trying to add a new client to an already used nickname");
	return ;
}



void							IRCServer::_addServer(AEntity &server, UnRegisteredConnection * executor)
{	
	if (server.getFamily() != SERVER_ENTITY_FAMILY)
	{
		Logger::critical("bad entity insertion: trying to add a non-server family entity to _addServer()");
		return ;
	}

	if (executor != NULL) // RELAYED
	{
		this->_unregistered_connections.erase(&executor->getStream());
		this->_connections.erase(&executor->getStream());
		delete executor;
	}
	if (!this->_entities.insert(std::make_pair(server.getUID(), &server)).second)
		Logger::critical("double insertion in _entities: trying to add a new server to an already used nickname");
	else if (!this->_servers.insert(std::make_pair(server.getUID(), &server)).second)
		Logger::critical("double insertion in _servers: trying to add a new server to an already used nickname");
	else if (!(server.getType() & RelayedEntity::value_type) && !this->_connections.insert(std::make_pair(&static_cast<Server &>(server).getStream(), reinterpret_cast<NetworkEntity *>(&server))).second)
		Logger::critical("double insertion in _connections: trying to add a new server to an already used nickname");
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





void							IRCServer::_sendMessage(AEntity & target, const std::string &message, AEntity *except)
{
	int type = target.getType() & ~AEntity::value_type & ~NetworkEntity::value_type & ~RelayedEntity::value_type;// TODO ~ error
	switch (type)
	{
		case Channel::value_type :
		{
			Logger::debug("Sending channel message");
			Package package(this->_protocol, this->_protocol.format(message));
			reinterpret_cast<Channel*>(&target)->broadcastPackage(package, except ? &reinterpret_cast<NetworkEntity*>(except)->getStream() : NULL);
			break;
		}
		case Client::value_type :
		{
			Logger::debug("Sending Client message: " + message);
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
			Logger::debug("Sending Server message: " + message);
			Package *package = new Package(this->_protocol, this->_protocol.format(message), &reinterpret_cast<Server*>(&target)->getStream());
			this->sendPackage(package, reinterpret_cast<Server*>(&target)->getStream());
			break;
		}
		case Server::value_type_forward :
		{
			Logger::debug("Sending Forward Server message: " + message);
			Package *package = new Package(this->_protocol, this->_protocol.format(message), &reinterpret_cast<Server*>(&target)->getStream());
			this->sendServerPackage(package, reinterpret_cast<Server*>(&target)->getStream());
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



void							IRCServer::_sendMessage(AEntity & target, const std::stringstream &message, AEntity *except)
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

void							IRCServer::_sendAllClients(const std::string &message, AEntity *except)
{
	for (std::map<std::string, AEntity *>::iterator it = this->_clients.begin(); 
	it != this->_clients.end(); 
	++it)
	{
		if (except && except->getUID() == it->second->getUID())
			continue ;
		Package *pkg;
		if (it->second->getType() & Client::value_type)
		{
			SockStream& sock = static_cast<SockStream&>(reinterpret_cast<Client*>(it->second)->getStream());
			pkg = new Package(this->_protocol, this->_protocol.format(message), &sock);
			this->sendPackage(pkg, sock);
		}
		else if (it->second->getType() & RelayedClient::value_type)
		{
			SockStream& sock = static_cast<SockStream&>(reinterpret_cast<RelayedClient*>(it->second)->getServer().getStream());
			pkg = new Package(this->_protocol, this->_protocol.format(message), &sock);
			this->sendPackage(pkg, sock);
		}
		else
			Logger::critical("non client family entity in server list");
	}
}

void							IRCServer::_sendAllServers(const std::string &message, AEntity *except)
{
	for (std::map<std::string, AEntity *>::iterator it = this->_servers.begin();
	it != this->_servers.end(); 
	++it)
	{
		if (except && except->getUID() == it->second->getUID())
			continue ;
		Package *pkg;
		if (it->second->getType() & (Server::value_type | Server::value_type_forward))
		{
			SockStream& sock = static_cast<SockStream&>(reinterpret_cast<Server*>(it->second)->getStream());
			pkg = new Package(this->_protocol, this->_protocol.format(message), &sock);
			this->sendPackage(pkg, sock);
		}
	}
}

/*
** ----------------------------- Server events ----------------------------------
*/




void						IRCServer::_onClientJoin(SockStream & s)
{
	UnRegisteredConnection *connection(new UnRegisteredConnection(s));
	this->_unregistered_connections.insert(std::make_pair(&s, connection));
	this->_connections.insert(std::make_pair(&s, connection));
	Logger::info("new incomming connection from " + s.getHost());
	this->_printServerState();
}




void							IRCServer::_onClientRecv(SockStream & s, Package & pkg)
{
	NetworkEntity *entity = getEntityByStream(s);
	if (!entity || pkg.getData().empty())
		return ;
	std::string payload = pkg.getData();
	uint ret = this->_handler.handle(*entity, payload);
	if (ret)
		Logger::error("onClientRecv: Something bad happened in handler");
}




// TODO REFRACTOR ALL
void							IRCServer::_onClientQuit(SockStream &s)
{
	NetworkEntity*	nEntity	= getEntityByStream(s);
	if (nEntity == NULL)
	{
		Logger::critical("ClientQuit: No entity corresponding was found.");
		return ;
	}
	if (nEntity->getType() & (Server::value_type | Server::value_type_forward))
	{
		Server	*srv = reinterpret_cast<Server*>(nEntity);
		Logger::warning("lost connection from server (" + srv->getUID() + "!" + srv->getName() + "@" + srv->getHostname() + ")");
		this->_sendAllServers(":" + srv->getUID() + " SQUIT " + srv->getUID() + " :netsplit occured.");
		for (std::map<std::string, AEntity*>::iterator it = this->_servers.begin(); it != this->_servers.end(); )
		{
			if (it->second->getType() & RelayedServer::value_type)
			{
				RelayedServer *relay_srv = reinterpret_cast<RelayedServer*>(it->second);
				++it;
				if (relay_srv->getServer().getStream().getSocket() == nEntity->getStream().getSocket())
				{
					/* netsplit on that relayed server */
					Logger::warning("netsplit occured ! lost route to server: "
						+ relay_srv->getUID() + "!" + relay_srv->getName() + "@" + relay_srv->getHostname());
					this->_sendAllServers(":" + this->getUID() + " SQUIT " + relay_srv->getUID() + " :netsplit occured.");
					this->_servers.erase(relay_srv->getUID());
					this->_entities.erase(relay_srv->getUID());
					delete relay_srv;
				}
			}
			else ++it;
		}
		/* drop all client connections related to this server */
		for (std::map<std::string, AEntity*>::iterator it = this->_clients.begin(); it != this->_clients.end(); )
		{
			if (it->second->getType() & RelayedClient::value_type)
			{
				RelayedClient *relayedClient = reinterpret_cast<RelayedClient*>(it->second);
				++it;
				if (relayedClient->getServer().getStream().getSocket() == nEntity->getStream().getSocket())
				{
					/* client netsplited on that server */
					Logger::warning("netsplit occured ! drop connection with: " + relayedClient->getIdent());
					this->_sendAllServers(relayedClient->getPrefix() + " QUIT " + relayedClient->getUID() + " :netsplit occured.");
					this->_clients.erase(relayedClient->getUID());
					this->_entities.erase(relayedClient->getUID());
					delete relayedClient;
				}
			}
			else ++it;
		}
	}
	else if (nEntity->getType() & Client::value_type)
	{
		reinterpret_cast<Client*>(nEntity)->leaveAllChannels("disconnected");
	}
	this->_entities.erase(nEntity->getUID());
	this->_clients.erase(nEntity->getUID());
	this->_servers.erase(nEntity->getUID());
	this->_connections.erase(&nEntity->getStream());
	this->_unregistered_connections.erase(&nEntity->getStream());
	delete nEntity;
}




/***** Client events *****/



void						IRCServer::_onRecv(SockStream &server, Package &pkg)
{
	NetworkEntity *entity = getEntityByStream(server);
	if (!entity || pkg.getData().empty())
		return ;
	std::string payload = pkg.getData();
	uint ret = this->_handler.handle(*entity, payload);
	if (ret)
		Logger::error("onRecv: Something bad happened in handler");
}




void				IRCServer::_onConnect ( SockStream &server)
{
	Logger::info("Connecting to forward server");
	UnRegisteredConnection *forward = this->_unregistered_connections.insert(std::make_pair(&server, new UnRegisteredConnection(server))).first->second;
	forward->setPassword(this->_password);
	std::stringstream ss;
	if (this->_forwardPassword.empty())
		ss << "PASS " << this->_password << " " << this->getVersion() << " " << this->getFlags();
	else
		ss << "PASS " << this->_forwardPassword << " " << this->getVersion() << " " << this->getFlags();
	this->_sendMessage(server, ss);
	ss.str("");
	ss << "SERVER " << this->_name << " 0 " << this->getUID() << " :" << this->_info; 
	this->_sendMessage(server, ss);
}




/* A connection with forward server is dropped*/
void				IRCServer::_onQuit( SockStream &server)
{
	NetworkEntity*	nEntity	= getEntityByStream(server);
	if (nEntity == NULL)
	{
		Logger::critical("ClientQuit: No entity corresponding was found.");
		return ;
	}
	/* get corresponding entity*/
	if ((nEntity->getType() & Server::value_type_forward) == false)
	{
		//TODO Do something here for this message. I don't know what, it is 7A.M ^^
		Logger::critical("ClientQuit: Corresponding entity is not a forward server.");
		return ;
	}
	Server	*srv = reinterpret_cast<Server*>(nEntity);
	/* get corresponding server*/
	Logger::warning("lost connection from server (" + srv->getUID() + "!" + srv->getName() + "@" + srv->getHostname() + ")");
	this->_sendAllServers(":" + srv->getUID() + " SQUIT " + srv->getUID() + " :netsplit occured.");
	/* drop all server connections related */
	for (std::map<std::string, AEntity*>::iterator it = this->_servers.begin(); it != this->_servers.end(); )
	{
		if (it->second->getType() & RelayedServer::value_type)
		{
			RelayedServer *relay_srv = reinterpret_cast<RelayedServer*>(it->second);
			++it;
			if (relay_srv->getServer().getStream().getSocket() == nEntity->getStream().getSocket())
			{
				/* netsplit on that relayed server */
				Logger::warning("netsplit occured ! lost route to forward server: "
					+ relay_srv->getUID() + "!" + relay_srv->getName() + "@" + relay_srv->getHostname());
				this->_sendAllServers(":" + this->getUID() + " SQUIT " + relay_srv->getUID() + " :netsplit occured.");
				this->_servers.erase(relay_srv->getUID());
				this->_entities.erase(relay_srv->getUID());
				delete relay_srv;
			}
		}
		else ++it;
	}
	/* drop all client connections related */
	for (std::map<std::string, AEntity*>::iterator it = this->_clients.begin(); it != this->_clients.end(); )
	{
		if (it->second->getType() & RelayedClient::value_type)
		{
			RelayedClient *relayedClient = reinterpret_cast<RelayedClient*>(it->second);
			++it;
			if (relayedClient->getServer().getStream().getSocket() == nEntity->getStream().getSocket())
			{
				/* client netsplited on that server */
				Logger::warning("netsplit occured ! drop connection with: " + relayedClient->getIdent());
				this->_sendAllServers(relayedClient->getPrefix() + " QUIT " + relayedClient->getUID() + " :netsplit occured.");
				this->_clients.erase(relayedClient->getUID());
				this->_entities.erase(relayedClient->getUID());
				delete relayedClient;
			}
		}
		else ++it;
	}
	this->_entities.erase(nEntity->getUID());
	this->_servers.erase(nEntity->getUID());
	this->_connections.erase(&nEntity->getStream());
	this->_unregistered_connections.erase(&nEntity->getStream());
	delete nEntity;

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



std::string					IRCServer::getCreationDate( void ) const
{
	return std::ctime(&this->_creationTime);
}


std::string					IRCServer::getMotdsPath( void ) const
{
	return "./conf";
}


void						IRCServer::setDebugLevel( bool debug )
{
	this->_debugLevel = debug;
}


uint						IRCServer::getDebugLevel( void ) const
{
	return this->_debugLevel;
}

/*
** --------------------------------- COMMANDS ---------------------------------
*/




void							IRCServer::_initCommands( void )
{
	this->_handler.addCommand<CommandPass>("PASS");
	this->_handler.addCommand<CommandNick>("NICK");
	this->_handler.addCommand<CommandUser>("USER");
	this->_handler.addCommand<CommandPrivmsg>("PRIVMSG");
	this->_handler.addCommand<CommandJoin>("JOIN");
	this->_handler.addCommand<CommandServer>("SERVER");
	this->_handler.addCommand<CommandError>("ERROR");
	this->_handler.addCommand<CommandMode>("MODE");
	this->_handler.addCommand<CommandQuit>("QUIT");
	this->_handler.addCommand<CommandSquit>("SQUIT");
	this->_handler.addCommand<CommandOper>("OPER");
	this->_handler.addCommand<CommandMotd>("MOTD");
	this->_handler.addCommand<CommandConnect>("CONNECT");
	this->_handler.addCommand<CommandDie>("DIE");
	this->_handler.addCommand<CommandPong>("PONG");
	this->_handler.addCommand<CommandPart>("PART");
	this->_handler.addCommand<CommandVersion>("VERSION");
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




// prefix format -> Server-Server: <server_uid>SPACE
// prefix format -> Server-Client: <nickname>[!<username>@<host_uid>]SPACE
std::string							IRCServer::makePrefix(const AEntity *user, const AEntity *host_server)
{
	std::string prefix = ":";

	if (user && host_server 
	&& (user->getType() & Client::value_type || user->getType() & RelayedClient::value_type)
	&& (host_server->getType() & Server::value_type || host_server->getType() & RelayedServer::value_type))
	{
		prefix += user->getUID() + "!" + user->getName() + "@" + host_server->getUID() + " ";
	}
	else
	{
		prefix += this->getUID() + " ";
	}
	return (prefix);
}




// prefix format -> Server-Server: <server_uid>SPACE
// prefix format -> Server-Client: <nickname>[!<username>@<host_uid>]SPACE
bool								IRCServer::parsePrefix(NetworkEntity & executor, const std::string &prefix,  RelayedServer **const host_server, AEntity **const emitter, std::string *username)
{
	if (prefix.find(":") != 0 || !host_server)
	{
		*emitter = &executor;
		return (false);
	}
	/* extended prefix */
	if (prefix.find("@") != std::string::npos && emitter && host_server) //REVIEW no host_server here (can have @ witout !)
	{
		/* parse sender */
		if (prefix.find("@") == std::string::npos)
			Logger::debug("@ not found");
		std::string uname;
		std::string firstName;
		std::string secondName; 
		if (prefix.find("!") != std::string::npos)
		{
			Logger::debug("Parse extented prefix");
			firstName = prefix.substr(1, prefix.find ("!") - 1);
			Logger::debug("Parse firstname: " + firstName);
			uname = prefix.substr(prefix.find("!") + 1, prefix.find("@") - 1);
			Logger::debug("Parse username: " + uname);
			secondName = prefix.substr(prefix.find("@") + 1, prefix.size());
			Logger::debug("Parse secondName: " + secondName);
		}
		else
		{
			Logger::debug("Parse extented prefix");
			firstName = prefix.substr(1, prefix.find ("@") - 1);
			Logger::debug("Parse firstname: " + firstName);
			secondName = prefix.substr(prefix.find("@") + 1, prefix.size());
			Logger::debug("Parse secondName: " + secondName);
		}
		
		if (this->_clients.count(firstName) != 1 )
		{
			Logger::critical("Unknown user in prefix: " + firstName);
			*emitter = &executor;
			return (false);
		}
		// else if (this->_clients[firstName]->getType() & RelayedClient::value_type) == false)
		// {
		// 	Logger::critical("Prefix original sender is not a relayed client");
		// 	return false;
		// }
		*emitter = this->_clients[firstName];
		*username = uname;
		// if (firstName != this->getUID()) //REVIEW is always needed?
		// {
		// 	if (this->_servers.count(secondName) == 0)
		// 	{
		// 		//TODO try with getting IP  from hostname (can receive z4r7p4.42lyon.fr and it's valid)
		// 		Logger::critical("Unknown host in prefix: " + secondName);
		// 		return (false);
		// 	}
		// 	*host_server = reinterpret_cast<RelayedServer*>(this->_servers[secondName]);
		// }
	}
	/* simple prefix */
	else
	{
		Logger::debug("Simple prefix");
		*emitter = NULL;
		std::string uid = prefix.substr(1, prefix.size() - 1);
		Logger::debug("Prefix uid: " + uid);
		if (this->_servers.count(uid) == 0) 
		{
			if (this->_clients.count(uid) == 0)
			{
				Logger::critical("Unknown server/client in prefix: " + uid);
				*emitter = &executor;
				return (false);
			}
			else if (this->_clients[uid]->getType() & RelayedClient::value_type)
			{
				Logger::debug("client in prefix is Relayed: " + uid);
				*emitter = this->_clients[uid];
			}
			else
			{
				Logger::debug("client in prefix is Relayed: " + uid);
				*emitter = this->_clients[uid];	
			}
		}
		else if (this->_servers[uid]->getType() & RelayedServer::value_type)
		{
			Logger::debug("Server in prefix is Relayed" + uid);
			*emitter =this->_servers[uid];
		}
		else
		{
			Logger::debug("Server in prefix is a local" + uid);
			*emitter =this->_servers[uid];
		}
	}
	return (true);
}

/* ************************************************************************** */