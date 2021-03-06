#include "IRCServer.hpp"

const uint				IRCServer::value_type = Server::value_type;

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
// TODO set server token, name & info 
//REVIEW super idée: une fonction: executeAfterTimeout() qui permettrait de lancer un thread qui executera une fonction après un certain temps (genre setTimeout de js mais en cpp)
IRCServer::IRCServer(ushort port, const std::string & password, const std::string &host, const std::string &ssl_cert_path, const std::string &ssl_key_path, const ushort tls_port)
:	ASockManager(ssl_cert_path, ssl_key_path),
	ANode(host),
	AEntity(IRCServer::value_type, "token", &Logger::getInitialTimestamp()),
	ServerInfo("name", ntos(IRC_CURRENT_VERSION), "default hostname", "IRC|amazircd"),
	_handler(*this),
	_protocol(),
	_forwardPassword(""),
	_operName("becomeOper"),
	_operPassword("becomeOper"),
	_shortMotdEnabled(true),
	_debugLevel(0)
{
	this->_initCommands();
	this->setPassword(password);

	if (this->listenOn(port, this->_protocol) == false)
		throw IRCServer::InitialisationException();

	/* if key and cert are specified, listen on tls port */
	if (!ssl_cert_path.empty() && !ssl_key_path.empty())
		if (this->listenOn(tls_port, this->_protocol, true) == false)
			throw IRCServer::InitialisationException();
	
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
	/* delete all entities */
	for (std::map<std::string, AEntity *>::iterator it = this->_entities.begin();
		it != this->_entities.end();
		++it)
	{
		delete (*it).second;
	}

	/* delete all unregistered connections */
	for (std::map<SockStream*, UnRegisteredConnection*>::iterator it = this->_unregistered_connections.begin();
		it != this->_unregistered_connections.end();
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
		for (std::list<AEntity *>::iterator itc = it->second->clientBegin(); itc != it->second->clientEnd(); ++itc)
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
	Logger::debug("--- clients ---");
	for (std::map<std::string, AEntity *>::const_iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (it->second->getType() & Client::value_type)
		{
			ClientInfo &client = static_cast<ClientInfo&>(*reinterpret_cast<Client *>(it->second));
			Logger::debug("-- " + it->second->getUID() + 
				"\n\t - name     : " + it->second->getName() +
				"\n\t - realname : " + client.getRealname() +
				"\n\t - host     : " + client.getHostname() +
				"\n\t - server   : " + client.getServerToken() +
				"\n\t - isOP     : " + ntos(client.isServerOP()) +
				"\n\t - nbChan   : " + ntos(client.getConcurrentChannels()) +
				"\n\t - nbChanMax: " + ntos(client.getConcurrentChannelsMax())
			);
		}
		else
		{
			ClientInfo &client = static_cast<ClientInfo&>(*reinterpret_cast<RelayedClient *>(it->second));
			Logger::debug("-- " + it->second->getUID() + 
				"\n\t - name     : " + it->second->getName() +
				"\n\t - realname : " + client.getRealname() +
				"\n\t - hop      : " + ntos(reinterpret_cast<RelayedClient*>(it->second)->getHopCount()) +
				"\n\t - host     : " + client.getHostname() +
				"\n\t - server   : " + client.getServerToken() +
				"\n\t - isOP     : " + ntos(client.isServerOP()) +
				"\n\t - nbChan   : " + ntos(client.getConcurrentChannels()) +
				"\n\t - nbChanMax: " + ntos(client.getConcurrentChannelsMax())
			);
		}
	}
	Logger::debug("--- unregistered ---");
	Logger::debug("nb unregistered connection: " + ntos(this->_unregistered_connections.size()));
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
		Logger::critical("double insertion in _clients: trying to add a new client to an already used nickname");
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




void							IRCServer::_sendMessage(AEntity & target, const std::string &message, AEntity *except)
{
	int			type = target.getType();
	SockStream*	socketTarget = NULL;

	Logger::debug("Sending to (" + target.getUID() + ") : (" + message + ")");
	if (type & Channel::value_type)
	{
		Package package(this->_protocol, this->_protocol.format(message));
		reinterpret_cast<Channel*>(&target)->broadcastPackage(package, except ? except->getUID() : "");	
		return ;
	}
	else if (type & NetworkEntity::value_type)
		socketTarget = &static_cast<NetworkEntity&>(target).getStream();
	else if (type & RelayedEntity::value_type)
		socketTarget = &static_cast<RelayedEntity&>(target).getServer().getStream();
	else
	{
		Logger::critical("Unknown entity type to send message: " + message);
		return ;
	}
	Package *package = new Package(this->_protocol, this->_protocol.format(message), socketTarget);
	this->sendPackage(package, *socketTarget);
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
	SockStream* sock = NULL;
	for (std::map<std::string, AEntity *>::iterator it = this->_clients.begin(); 
	it != this->_clients.end(); 
	++it)
	{
		if (except && except->getUID() == it->second->getUID())
			continue ;
		if (it->second->getType() & Client::value_type)
			sock = &reinterpret_cast<Client*>(it->second)->getStream();
		else if (it->second->getType() & RelayedClient::value_type)
			sock = &reinterpret_cast<RelayedClient*>(it->second)->getServer().getStream();
		else
			Logger::critical("non client family entity in server list");
		Package *pkg = new Package(this->_protocol, this->_protocol.format(message), sock);
		this->sendPackage(pkg, *sock);
	}
}

void							IRCServer::_sendAllServers(const std::string &message, AEntity *except)
{
	Package *pkg = NULL;
	for (std::map<std::string, AEntity *>::iterator it = this->_servers.begin();
	it != this->_servers.end(); 
	++it)
	{
		if (except && except->getUID() == it->second->getUID())
			continue ;
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
	UnRegisteredConnection *connection = new UnRegisteredConnection(s);
	this->_unregistered_connections.insert(std::make_pair(&s, connection));
	this->_connections.insert(std::make_pair(&s, connection));
	Logger::info("new incomming connection from " + s.getHost());
	if (this->getDebugLevel())
	{
		Logger::debug("after onClientJoin: ");
		this->_printServerState();
	}
}




void							IRCServer::_onClientRecv(SockStream & s, Package & pkg)
{
	NetworkEntity *entity = getEntityByStream(s);
	if (!entity)
	{
		Logger::critical("IRC: receive from an unknown entity");
		return ;
	}
	if (pkg.getData().empty())
		return ;
	std::string payload = pkg.getData();
	if (this->_handler.handle(*entity, payload))
		Logger::error("onClientRecv: Something bad happened in handler");
}




void							IRCServer::_onClientQuit(SockStream &s)
{
	NetworkEntity*	nEntity	= getEntityByStream(s);
	if (nEntity == NULL)
	{
		Logger::debug("ClientQuit: No entity corresponding was found.");
		return ;
	}
	Logger::info("disconnecting: " + nEntity->getUID() + "@" + nEntity->getStream().getHost());
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
					relayedClient->leaveAllChannels("netsplit occured");
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
		if (reinterpret_cast<Client*>(nEntity)->isCleanDisconnection() == false)
			this->_sendAllServers(reinterpret_cast<Client*>(nEntity)->getPrefix() + " QUIT :connection lost");
	}
	else if (nEntity->getType() & RelayedClient::value_type)
	{
		reinterpret_cast<RelayedClient*>(nEntity)->leaveAllChannels("disconnected");
	}
	this->_entities.erase(nEntity->getUID());
	this->_clients.erase(nEntity->getUID());
	this->_servers.erase(nEntity->getUID());
	this->_connections.erase(&nEntity->getStream());
	this->_unregistered_connections.erase(&nEntity->getStream());
	delete nEntity;
	if (this->getDebugLevel())
	{
		Logger::debug("after onClientQuit: ");
		this->_printServerState();
	}
}




/***** Client events *****/



void						IRCServer::_onRecv(SockStream &server, Package &pkg)
{
	NetworkEntity *entity = getEntityByStream(server);
	if (!entity) 
	{
		Logger::critical("IRC: receive from an unknown entity");
		return ;
	}
	else if (pkg.getData().empty())
		return ;
	std::string payload = pkg.getData();
	if (this->_handler.handle(*entity, payload))
		Logger::error("onRecv: Something bad happened in handler");
}




void				IRCServer::_onConnect ( SockStream &server)
{
	Logger::info("Connecting to forward server");
	//REVIEW why forward is not added to _connections map
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
	if (this->getDebugLevel())
	{
		Logger::debug("after onConnect");
		this->_printServerState();
	}
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
	if ((nEntity->getType() & UnRegisteredConnection::value_type))
	{
		Logger::info("Drop unsuccessfull connection to forward server");
		this->_unregistered_connections.erase(&nEntity->getStream());
		delete nEntity;
		return ;
	}
	else if ((nEntity->getType() & Server::value_type_forward) == false)
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
				relayedClient->leaveAllChannels("netsplit occurred");
				this->_sendAllServers(relayedClient->getPrefix() + " QUIT " + relayedClient->getUID() + " :netsplit occured.");
				this->_clients.erase(relayedClient->getUID());
				this->_entities.erase(relayedClient->getUID());
				delete relayedClient;
			}
		}
		else ++it;
	}
	this->_forgetLocalServer(*srv);
	if (this->getDebugLevel())
	{
		Logger::debug("after onQuit: ");
		this->_printServerState();
	}
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



std::string					IRCServer::getMotdsPath( void ) const
{
	return IRC_DEFAULT_MOTD_PATH;
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
	//REVIEW remove mode message? 
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
	this->_handler.addCommand<CommandKill>("KILL");
	this->_handler.addCommand<CommandNotice>("NOTICE");
	this->_handler.addCommand<CommandNjoin>("NJOIN");
	this->_handler.addCommand<CommandTopic>("TOPIC");
	this->_handler.addCommand<CommandList>("LIST");
	this->_handler.addCommand<CommandKick>("KICK");
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
		std::string	uname;
		std::string	firstName;
		std::string	secondName; 
		/* parse sender */
		if (prefix.find("@") == std::string::npos)
			Logger::debug("@ not found");
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
		
		if (this->_clients.count(firstName) != 1)
		{
			Logger::critical("Unknown user in prefix: " + firstName);
			*emitter = &executor;
			return (false);
		}
		*emitter = this->_clients[firstName];
		*username = uname;
	}
	/* simple prefix */
	else
	{
		Logger::debug("Simple prefix");
		*emitter = NULL;
		std::string uid = prefix.substr(1, prefix.size() - 1);
		Logger::debug("Prefix uid: " + uid);
		if (uid == this->getUID())
		{
			Logger::warning("Self executed request");
			*emitter = this;
			return (true);
		}
		else if (this->_servers.count(uid) == 0) 
		{
			if (this->_clients.count(uid) == 0)
			{
				Logger::warning("Unknown server/client in prefix: <" + uid + "> (normal if coming from forward's first message)");
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
			*emitter = this->_servers[uid];
		}
	}
	return (true);
}

/*
** --------------------------------- STATE ---------------------------------
*/


void		IRCServer::_forgetLocalServer(Server & srv)
{
	this->_entities.erase(srv.getUID());
	this->_servers.erase(srv.getUID());
	this->_connections.erase(&srv.getStream());
	delete &srv;
}

/* ************************************************************************** */