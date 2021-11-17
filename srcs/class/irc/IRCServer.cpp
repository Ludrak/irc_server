#include "IRCServer.hpp"

const uint				IRCServer::value_type = 1333;

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

// // TODO REFRACTOR 
// bool							IRCServer::setNetworkConnection(const std::string & host, ushort port, std::string & password)
// {
// 	// this->_forword_socket = new SockStream(host, port);
// 	Logger::info("Try connecting to network:"); 
// 	Logger::info("- host     : " + ntos(host)); 
// 	Logger::info("- port     : " + ntos(port)); 
// 	Logger::info("- password : " + ntos(password));
// 	try
// 	{
// 		if (this->connectOn(host, port, this->_protocol) == false)
// 		{
// 			Logger::error("Cannot connect to network");
// 			return false;
// 		}
// 	}
// 	catch (const AClient::ConnectionException &e)
// 	{
// 		// connection exception, we are root on that server
// 		Logger::info ("Forward connection failed: running server as root node");
// 		return false;
// 	}
// 	return true;
// }



// REVIEW REFRACTORED
AEntity						*IRCServer::_registerClient(AEntity & entity, int type)
{
	int t = type & ~AEntity::value_type & ~NetworkEntity::value_type & ~RelayedEntity::value_type;
	switch (t)
	{
		/* register client from UnRegisteredConnection */
		/* UnRegisteredConnection must have all infomations to create a client now */
		case Client::value_type:
		{
			//REVIEW use reinterpret_cast here instead of dynamic_cast
			UnRegisteredConnection *connection = dynamic_cast<UnRegisteredConnection*>(&entity);
			if (connection == NULL)
			{
				Logger::critical("client registered from unregistered connection isn't already connected");
				return NULL;
			}
			Client	*client = new Client(*connection);
			this->_clients.insert(std::make_pair(client->getUID(), client));
			this->_entities.insert(std::make_pair(client->getUID(), client));
			//TODO delete the lost UnRegisteredConnection
			this->_unregistered_connections.erase(&connection->getStream());
			Logger::info ("new user joined : " + client->getName() + " (" + client->getUID() + "@" + client->getStream().getIP() + ")");
			return client;
		}
		
		/* register client from RelayedClient */
		/* in here we assume that the RelayedClient has successfully been created */
		case RelayedClient::value_type:
		{
			//REVIEW use reinterpret_cast here instead of dynamic_cast
			RelayedClient *relay_client = dynamic_cast<RelayedClient*>(&entity);
			this->_clients.insert(std::make_pair(relay_client->getUID(), relay_client));
			this->_entities.insert(std::make_pair(relay_client->getUID(), relay_client));
			Logger::info ("new user joined : " + relay_client->getName() + " (" + relay_client->getUID() + "@" + relay_client->getServer().getStream().getIP() + " is " + ntos(relay_client->getHopCount()) + " hops(s) away)");
			return relay_client;
		}
		default:
			Logger::critical("setting as registered a client with an unknown type: " + ntos(type));
			return NULL;
	}
	return NULL;
}

//REVIEW NEW
void							IRCServer::_registerServer(AEntity &entity, const int type)
{
	int t = type & ~AEntity::value_type & ~NetworkEntity::value_type & ~RelayedEntity::value_type;
	switch (t)
	{
		/* register server from UnRegisteredConnection */
		/* UnRegisteredConnection must have all infomations to create a server now */
		case Server::value_type:
		{
			UnRegisteredConnection *connection = dynamic_cast<UnRegisteredConnection*>(&entity);
			if (connection == NULL)
			{
				Logger::critical("client registered from unregistered connection isn't already connected");
				return ;
			}
			Server *server = new Server(*connection);
			this->_servers.insert(std::make_pair(server->getUID(), server));
			this->_entities.insert(std::make_pair(server->getUID(), server));
			this->_unregistered_connections.erase(&connection->getStream());
			Logger::info ("new server connected to network : " + server->getName() + " (" + server->getUID() + "@" + server->getStream().getIP() + ")");
			break;
		}

		/* register server from RelayedServer */
		/* in here we assume that the RelayedServer has successfully been created */
		case RelayedServer::value_type:
		{
			RelayedServer *relay_server = dynamic_cast<RelayedServer*>(&entity);
			this->_servers.insert(std::make_pair(relay_server->getUID(), relay_server));
			this->_entities.insert(std::make_pair(relay_server->getUID(), relay_server));
			Logger::info ("new server connected to network : " + relay_server->getName() + " (" + relay_server->getUID() + "@" + relay_server->getServer().getStream().getIP() + " is " + ntos(relay_server->getHopCount()) + " hops(s) away)");
			break;
		}

		default:
			Logger::critical("setting as registered a server with an unknown type: " + ntos(type));
			return ;
	}
}

// TODO REFRACTOR ALL
// REVIEW UGLY REFRACTORED VERSION
void							IRCServer::_sendMessage(AEntity & target, const std::string &message, const AEntity *except)
{
	//TODO add server prefix
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
** --------------------------------- EVENTS ----------------------------------
*/

/***** Server events *****/

// REVIEW REFRACTORED
void						IRCServer::_onClientJoin(SockStream & s)
{
	UnRegisteredConnection *connection(new UnRegisteredConnection(s));
	this->_unregistered_connections.insert(std::make_pair(&s, connection));
	this->_connections.insert(std::make_pair(&s, connection));
	Logger::info("new incomming connection from " + s.getIP());
	this->_printServerState();
}


// REVIEW REFRACTORED
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
	Logger::debug("Receiving package from forward server");
	Logger::debug("data: " + pkg.getData() );
}

// TODO REFRACTOR AND HANDLE FORWARD
void				IRCServer::_onConnect ( SockStream &server)
{
	(void)server;
	Logger::info("Connecting to forward server");
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
// REVIEW REFRACTORED
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

// REVIEW NEW
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

// REVIEW NEW
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

// REVIEW REFRACTORED FOR NEW ARCH                   
// INFO : we need to be able to execute any command for any clients type
// INFO : RelayedClient and RelayedServer just need to send back the command to target server
// int			IRCServer::execute(AEntity *e, std::string data)
// {
// 	Server			*sender = NULL;
// 	AEntity			*client = NULL;
// 	AEntity			*client_host = NULL;
// 	if (data[0] == ':')
// 	{
// 		this->parsePrefix(data.substr(0, data.find(" ")), &sender, reinterpret_cast<RelayedClient**>(&client), reinterpret_cast<RelayedServer**>(&client_host));
// 		data = data.substr(data.find(" ") + 1, data.size() - data.find(" ") - 1);
// 		if (sender == NULL)
// 			return (1);
// 	}
// 	std::string command = data.substr(0, data.find(" "));

// 	if (e->getType() & Client::value_type || client)
// 	{
// 		/* no prefix defined so we use executor */
// 		if (!client)
// 			client = reinterpret_cast<Client*>(e);
		
// 		/* check if command is part of list of operations */
// 		if (this->_userCommands.count(command) == 0)
// 		{
// 			if (client->getType() & Client::value_type)
// 			{
// 				Logger::debug("user sent command not found (" + client->getUID() + "@" + reinterpret_cast<Client*>(client)->getStream().getIP() + ")");
// 				std::stringstream ss(ERR_UNKNOWNCOMMAND(data)); 
// 				this->_sendMessage(reinterpret_cast<Client*>(client)->getStream(), ss);
// 			}
// 			else if (client->getType() & RelayedClient::value_type)
// 			{
// 				Logger::debug("relayed user sent unknown command (" + client->getUID() + "@" + reinterpret_cast<RelayedClient*>(client)->getServer().getStream().getIP() + ")");
// 				std::stringstream ss(ERR_UNKNOWNCOMMAND(data)); 
// 				this->_sendMessage(reinterpret_cast<RelayedClient*>(client)->getServer().getStream(), ss);
// 			}
// 			return (1);
// 		}
// 		/* execute command from command list */
// 		return ((this->*(_userCommands[command]))(*client, data.substr(command.size() + 1, data.size() - (command.size() + 1))));
// 	}
// 	else if ((e->getFamily() & SERVER_ENTITY_FAMILY) && sender)
// 	{
// 		/* check if command is part of list of operations */
// 		if (this->_serverCommands.count(command) == 0)
// 		{
// 			if (e->getType() & Server::value_type)
// 			{
// 				Logger::debug("server sent unknown command (" + e->getUID() + "@" + reinterpret_cast<Server*>(client)->getStream().getIP() + ")");
// 				std::stringstream ss(ERR_UNKNOWNCOMMAND(data)); 
// 				this->_sendMessage(reinterpret_cast<Server*>(client)->getStream(), ss);
// 			}
// 			else if (client->getType() == RelayedServer::value_type)
// 			{
// 				Logger::debug("relayed server sent unknown command  (" + client->getUID() + "@" + reinterpret_cast<RelayedServer*>(client)->getServer().getStream().getIP() + ")");
// 				std::stringstream ss(ERR_UNKNOWNCOMMAND(data)); 
// 				this->_sendMessage(reinterpret_cast<RelayedServer*>(client)->getServer().getStream(), ss);
// 			}
// 			return (1);
// 		}
// 		/* execute command from command list */
// 		return ((this->*(_serverCommands[command]))(*client, data.substr(command.size() + 1, data.size() - (command.size() + 1))));
// 	}
// 	// TODO ADD UNREGISTERED                                
// 	// TODO find a way to fit all 3 client types in one call
// 	Logger::critical("Command '" + command + "' has no viable executor");
// 	return (1);
// }

// TODO REFRACTOR ALL
// TODO (still) move away ! 
// uint		IRCServer::_commandSERVER(Client & client, std::string cmd)
// {
// 	//TODO see what to do when no param specified
// 	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<SERVER> with args: " + cmd );
// 	if (Parser::nbParam(cmd) < 4)
// 		return SUCCESS;
// 	std::string	serverName = Parser::getParam(cmd, 0);
// 	int			hopCount = std::stoi(Parser::getParam(cmd, 1));
// 	std::string	info = Parser::getParam(cmd, 3);
	
// 	uint token = 0;
// 	try
// 	{
// 		token = std::stoi(Parser::getParam(cmd, 2));
// 		if (token > 999)
// 			throw ;
// 	}
// 	catch(const std::exception& e)
// 	{
// 		// TODO invalid token ?
// 		Logger::error ("Server trying to connect has invalid token: \"" + Parser::getParam(cmd, 2) + "\"");
// 		return (SUCCESS);
// 	}

// 	if (token == this->_token || this->_ircClients.count(std::to_string (token)) == 1)
// 	{
// 		Logger::error("already regristered server ! (" + client.getStream().getIP() + ")");
// 		Logger::error("closing connection to " + client.getStream().getIP() + " to preserve network stability.");
// 		//TODO this->disconnect(client.getStream());
// 		return ERR_ALREADYREGISTRED;
// 	}
// 	if (client.getType() == Client::value_type_unknown)
// 	{
// 		if (hopCount != 0)
// 		{
// 			/* cannot introduce new server behind while not regristered */
// 			Logger::error ("unregistered client tries to introduce new server behind himself");
// 			return (SUCCESS); // ?
// 		}
// 		else
// 		{
// 			client.setNickname(serverName);
// 			client.setHopCount(hopCount);
// 			client.setSID(std::to_string(token));
// 			client.setServerDescription(info);
// 			this->_setRegistered(client, Client::value_type_server);
// 			this->_printServerState();
// 			Logger::info("new server connected to network: " + serverName + " (" + client.getSID() + "@" + client.getStream().getIP() + ")");
// 		}
// 	}
// 	else if (hopCount == 0)
// 	{

// 		/* server trying to register twice */
// 		Logger::error("Server " + client.getSID() + "@" + client.getStream().getIP() + " tried to register twice");
// 		return (SUCCESS); // ?
// 	}
// 	else if (hopCount > 0)
// 	{
// 		/* 
// 		** if server_reference was registered by client we need to pass it a reference 
// 		** to it's sockstream so that we can communicate with it later easily
// 		*/ 
// 		Client *server_reference = new Client(client.getStream());
// 		server_reference->setNickname(serverName);
// 		server_reference->setHopCount(hopCount);
// 		server_reference->setSID(std::to_string(token));
// 		server_reference->setServerDescription(info);
// 		server_reference->setRelayed(true);
// 		this->_setRegistered(*server_reference, Client::value_type_server);
// 		this->_printServerState();
// 		Logger::info("new server connected to sub network: " + serverName + " (" + server_reference->getSID() + "@" + server_reference->getStream().getIP() + " is " + ntos(hopCount) + " hop(s) away)");
// 	}

// 	// relay to forward and all connected servers
// 	// meantime relay all infos about our connections to new server
// 	std::stringstream relay_all_ss;
// 	relay_all_ss << "SERVER " << serverName << " " << (hopCount + 1) << " " << token << " :" << info;
// 	if (this->_forwardSocket != 0)
// 	{
// 		Logger::debug("relaying server connection to forward server (" + this->_sockets[this->_forwardSocket]->getIP() + ")");
// 		this->_sendMessage(*this->_sockets[this->_forwardSocket], relay_all_ss);
// 		//relay_new_ss << "SERVER " <<  << " " << (hopCount + 1) << " " << token << " :" << info;
// 		// TODO SEND / RECV FORWARD INFORMATIONS
// 	}
// 	for (std::map<std::string, AEntity *>::iterator it = this->_ircClients.begin(); it != this->_ircClients.end(); it++)
// 	{
// 		if (it->second->getType() == Client::value_type_server && reinterpret_cast<Client*>(it->second)->getSID() != client.getSID())
// 		{
// 			Client *srv = reinterpret_cast<Client*>(it->second);
// 			Logger::debug("relaying server connection to connected server (" + srv->getStream().getIP() + ")");
// 			this->_sendMessage(*it->second, relay_all_ss);
// 			// TODO how to implement multiple backward relays
			
// 			//std::stringstream relay_new_ss;
// 			//relay_new_ss << "SERVER " << srv->getNickname() << " " << srv->getHopCount() + 1 << " " << srv->getSID() << " :" << srv->getServerDescription();
// 			//this->_sendMessage(*server_reference, relay_new_ss);
// 		}
// 	}
// 	return (SUCCESS);
// }

/* ************************************************************************** */