#include "IRCServer.hpp"

std::string				IRCServer::statusMessages[] = {};
const int				IRCServer::value_type = 1333;

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
//REVIEW Server name maximum 63 character
// TODO set server token, name & info 
IRCServer::IRCServer(ushort port, const std::string & password, const std::string &host)
: ASockManager(), ANode(host), AEntity(IRCServer::value_type, "token"), ServerInfo("name", "info", "pass"), _protocol()
{
	this->_initStatusMessages();
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


// TODO REFRACTOR
bool							IRCServer::_reply(Client & client, ushort statusCode, std::string target, std::string target2)
{
	if (statusCode > MAX_STATUS_MESSAGES)
	{
		Logger::critical("Try to send an invalid statusCode: " + ntos(statusCode));
		return !SUCCESS;
	}
	Logger::debug("Replying code: " + ntos(statusCode));
	Logger::debug("Raw response: " + IRCServer::statusMessages[statusCode]);
	Logger::debug("target: " + target);
	Logger::debug("target2: " + target2);
	std::string reply = Parser::formatReply(IRCServer::statusMessages[statusCode], target, target2);
	Logger::debug("Formated response: " + reply);
	// TODO set valid reply infos
	std::string prefix = ":" + client.getUID() + "!server-ident@sender-server ";
	std::stringstream ss;
	ss << prefix << ntos(statusCode) << " " << reply;
	this->_sendMessage(client, ss);
	return SUCCESS;
}


// REVIEW REFRACTORED
void							IRCServer::_registerClient(AEntity & entity, int type)
{
	switch (type)
	{
		/* register client from UnRegisteredConnection */
		/* UnRegisteredConnection must have all infomations to create a client now */
		case Client::value_type:
			UnRegisteredConnection *connection = dynamic_cast<UnRegisteredConnection*>(&entity);
			if (connection == NULL)
			{
				Logger::critical("client registered from unregistered connection isn't already connected");
				return ;
			}
			Client	*client = new Client(*connection);
			this->_clients.insert(std::make_pair(client->getUID(), client));
			this->_entities.insert(std::make_pair(client->getUID(), client));
			this->_unregistered_connections.erase(&connection->getStream());
			Logger::info ("new user joined : " + client->getName() + " (" + client->getUID() + "@" + client->getStream().getIP() + ")");
			break;
		/* register client from RelayedClient */
		/* in here we assume that the RelayedClient has successfully been created */
		case RelayedClient::value_type:
			RelayedClient *relay_client = dynamic_cast<RelayedClient*>(&entity);
			this->_clients.insert(std::make_pair(relay_client->getUID(), relay_client));
			this->_entities.insert(std::make_pair(relay_client->getUID(), relay_client));
			Logger::info ("new user joined : " + relay_client->getName() + " (" + relay_client->getUID() + "@" + relay_client->getRelayServer().getStream().getIP() + " is " + ntos(relay_client->getHopCount()) + " hops(s) away)");
			break;
		
		default:
			Logger::critical("setting as registered a client with an unknown type: " + ntos(type));
			return ;
	}
}

//REVIEW NEW
void							IRCServer::_registerServer(AEntity &entity, const int type)
{
	switch (type)
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
			Logger::info ("new server connected to network : " + relay_server->getName() + " (" + relay_server->getUID() + "@" + relay_server->getRelayServer().getStream().getIP() + " is " + ntos(relay_server->getHopCount()) + " hops(s) away)");
			break;
		}

		default:
			Logger::critical("setting as registered a server with an unknown type: " + ntos(type));
			return ;
	}
}

// TODO REFRACTOR ALL
// REVIEW UGLY REFRACTORED VERSION
void							IRCServer::_sendMessage(AEntity & target, const std::stringstream &message, const AEntity *except)
{
	//TODO add server prefix
	switch (target.getType())
	{
		case Channel::value_type :
		{
			Logger::debug("Sending channel message");
			Package package(this->_protocol, this->_protocol.format(message.str()));
			reinterpret_cast<Channel*>(&target)->broadcastPackage(package, &reinterpret_cast<const Client*>(except)->getStream());
			break;
		}
		case Client::value_type :
		{
			Logger::debug("Sending Client message");
			Package *package = new Package(this->_protocol, this->_protocol.format(message.str()), &reinterpret_cast<Client*>(&target)->getStream());
			this->sendPackage(package, reinterpret_cast<Client*>(&target)->getStream());
			break;
		}	
		case RelayedClient::value_type :
		{
			Logger::debug("Sending RelayedClient message");
			Package *package = new Package(this->_protocol, this->_protocol.format(message.str()), &reinterpret_cast<RelayedClient*>(&target)->getRelayServer().getStream());
			this->sendPackage(package, reinterpret_cast<RelayedClient*>(&target)->getRelayServer().getStream());
			break;
		}
		case Server::value_type :
		{
			Logger::debug("Sending Server message");
			Package *package = new Package(this->_protocol, this->_protocol.format(message.str()), &reinterpret_cast<Server*>(&target)->getStream());
			this->sendPackage(package, reinterpret_cast<Server*>(&target)->getStream());
			break;
		}
		case RelayedServer::value_type :
		{
			Logger::debug("Sending RelayedServer message");
			Package *package = new Package(this->_protocol, this->_protocol.format(message.str()), &reinterpret_cast<RelayedServer*>(&target)->getRelayServer().getStream());
			this->sendPackage(package, reinterpret_cast<RelayedServer*>(&target)->getRelayServer().getStream());
			break;
		}
		case UnRegisteredConnection::value_type :
		{
			Logger::debug("Sending UnregisteredConnection message");
			Package *package = new Package(this->_protocol, this->_protocol.format(message.str()), &reinterpret_cast<UnRegisteredConnection*>(&target)->getStream());
			this->sendPackage(package, reinterpret_cast<UnRegisteredConnection*>(&target)->getStream());
			break;
		}
		default:
			Logger::critical("Unknown entity type to send message: " + message.str());
	}
}

void							IRCServer::_sendMessage(SockStream & target, const std::stringstream &message)
{
	Package *pkg = new Package(this->_protocol, this->_protocol.format(message.str()), &target);
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
	AEntity *entity	= getEntityByStream(s);
	if (entity && !pkg.getData().empty())
		this->execute(entity, pkg.getData());
}


// TODO REFRACTOR ALL
void							IRCServer::_onClientQuit(SockStream &s)
{
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
	Logger::debug("Receiving package from forward server");
	Logger::debug("data: " + pkg.getData() );
}

// TODO REFRACTOR AND HANDLE FORWARD
void				IRCServer::_onConnect ( SockStream &server)
{
	Logger::info("Connecting to forward server");
	std::stringstream ss;
	ss << "SERVER " << this->_name << " 0 " << this->getUID() << " :" << this->_info; 
	this->_sendMessage(server, ss);
}

// TODO REFRACTOR AND HANDLE FORWARD
void				IRCServer::_onQuit( SockStream &server)
{
	Logger::warning("Stop connection to forward server");
	Logger::warning("TODO handle netsplit with forward network");
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
// REVIEW REFRACTORED
AEntity*						IRCServer::getEntityByStream(SockStream & s)
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

/*
** --------------------------------- COMMANDS ---------------------------------
*/

void							IRCServer::_initCommands( void )
{
	this->_unregisteredCommands.insert(std::make_pair("USER",	&IRCServer::_commandUSER));
	this->_unregisteredCommands.insert(std::make_pair("PASS",	&IRCServer::_commandPASS));
	this->_unregisteredCommands.insert(std::make_pair("NICK",	&IRCServer::_commandNICK));
	this->_unregisteredCommands.insert(std::make_pair("SERVER",	&IRCServer::_commandSERVER));

	this->_userCommands.insert(std::make_pair("USER",			&IRCServer::_commandUSER));
	this->_userCommands.insert(std::make_pair("PASS",			&IRCServer::_commandPASS));
	this->_userCommands.insert(std::make_pair("NICK",			&IRCServer::_commandNICK));
	this->_userCommands.insert(std::make_pair("PRIVMSG",		&IRCServer::_commandPRIVMSG));
	this->_userCommands.insert(std::make_pair("JOIN",			&IRCServer::_commandJOIN));
	this->_userCommands.insert(std::make_pair("MODE",			&IRCServer::_commandMODE));

	this->_serverCommands.insert(std::make_pair("SERVER",		&IRCServer::_commandSERVER));
}

// REVIEW NEW
// prefix format -> :<server_uid>[!<user_uid>@<host_uid>]SPACE
std::string							IRCServer::makePrefix(const AEntity *user=NULL, const AEntity *host_server=NULL)
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
bool								IRCServer::parsePrefix(const std::string &prefix, AEntity * *const sender_server, AEntity * *const user, AEntity * *const host_server)
{
	if (prefix.find(":") != 1)
		return (false);
	/* extended prefix */
	if (prefix.find("!") != std::string::npos && user && host_server)
	{
		/* parse sender */
		std::string token = prefix.substr(1, prefix.find ("!") - 1);
		if (this->_servers.count(token) != 1 || this->_servers[token]->getType() & ~NetworkEntity::value_type)
			return (false);
		*sender_server = this->_servers[token];

		/* parse sender client nickname */
		token = prefix.substr(prefix.find ("!") + 1, prefix.find("@") - prefix.find ("!") - 1);
		if (this->_clients.count(token) != 1)
			return (false);
		if (this->_clients[token]->getType() & ~RelayedClient::value_type)
		{
			Logger::error("extended prefix username is not a relayed client");
			return (false);
		}
		*user = this->_clients[token];

		/* parse host server */
		token = prefix.substr(prefix.find("@") + 1, prefix.size() - prefix.find("!") - 1);
		if (this->_servers.count(token) != 1
		|| this->_servers[token]->getType() & ~Server::value_type || this->_servers[token]->getType() & ~Server::value_type)
			return (false);
		*host_server = this->_servers[token];
	}
	/* simple prefix */
	else
	{
		std::string token = prefix.substr(1, prefix.size() - 1);
		if (this->_servers.count(token) != 1 || this->_servers[token]->getType() & ~NetworkEntity::value_type)
			return (false);
		*sender_server = this->_servers[token];
	}
	return (true);
}

// REVIEW OLD VERSION
// int								IRCServer::execute(AEntity & client, std::string data)
// {
// 	size_t sep = data.find(" ");
// 	std::string command(data);
// 	std::string args("");
// 	if (sep < data.size())
// 	{
// 		command = data.substr(0, sep);
// 		args = data.substr(sep + 1);
// 	}
// 	switch (client.getType())
// 	{
// 		case Client::value_type_client:
// 			if (this->_userCommands.count(command) == 1)
// 			{
// 				if ((this->*(this->_userCommands[command]))(dynamic_cast<Client&>(client), args) != SUCCESS)
// 					Logger::error("Error while executing command");
// 			}
// 			else
// 				Logger::warning("<" + client.getNickname() + ">: unknown command <" + command + ">");
// 			break;
// 		case Client::value_type_server:
// 			if (this->_serverCommands.count(command) == 1)
// 			{
// 				uint ret = (this->*(this->_serverCommands[command]))(dynamic_cast<Client&>(client), args);
// 				if (ret != 0)
// 				{
// 					std::string prefix = ":" + client.getNickname() + "!server-ident@sender-server ";
// 					std::stringstream ss;
// 					ss << prefix << ret;
// 					this->_sendMessage(client, ss); // senError
// 				}
// 			}
// 			else
// 				Logger::warning("unknown command");
// 			break;
// 		case Client::value_type_unknown:
// 			Logger::debug("yet unidentified client");
// 			if (this->_unregisteredCommands.count(command) == 1)
// 			{
// 				uint ret = (this->*(this->_unregisteredCommands[command]))(dynamic_cast<Client&>(client), args);
// 				if (ret != 0)
// 				{
// 					std::string prefix = ":" + client.getNickname() + "!server-ident@sender-server ";
// 					std::stringstream ss;
// 					ss << prefix << ret;
// 					this->_sendMessage(client, ss); // senError
// 				}
// 			}
// 			else
// 				Logger::warning(client.getNickname() + ": unknown command");
// 			break;
// 		default:
// 			Logger::error("unknown client type");
// 			break;
// 	}
// 	return 1;
// }

// REVIEW REFRACTORED FOR NEW ARCH                   
// TODO (still) REFRACTOR NICELY WITH PREFIX HANDLER & HANDLE ALL CLIENT TYPES
// INFO : we need to be able to execute any command for any clients type
// INFO : RelayedClient and RelayedServer just need to send back the command to target server
int			IRCServer::execute(AEntity *e, std::string data)
{
	std::string command = data.substr(0, data.find(" "));
	if (e->getType() == Client::value_type)
	{
		Client	*client = dynamic_cast<Client*>(e);
		if (this->_userCommands.count(command) == 1)
		{
			//TODO reply command not found
			Logger::debug("command not found (" + client->getUID() + "@" + client->getStream().getIP() + ")");
			return (1);
		}
		uint err = this->_userCommands[command](client, data.substr(command.size() + 1, data.size() - (command.size() + 1)));
		if (err != 0)
		{
			//TODO command err send reply
			Logger::debug("sending error " + ntos(err) + " to user " + client->getUID() + "@" + client->getStream().getIP());
			return (1);
		}
	}
	else if (e->getType() == Server::value_type || e->getType() == Server::value_type_forward)
	{
		Server	*server = dynamic_cast<Server*>(e);
		if (this->_userCommands.count(command) == 1)
		{
			//TODO reply command not found
			Logger::debug("server command not found (" + server->getUID() + "@" + server->getStream().getIP() + ")");
			return (1);
		}
		uint err = this->_userCommands[command](server, data.substr(command.size() + 1, data.size() - (command.size() + 1)));
		if (err != 0)
		{
			//TODO command err send reply
			Logger::debug("sending error " + ntos(err) + " to server " + server->getUID() + "@" + server->getStream().getIP());
			return (1);
		}
	}
	// TODO ADD UNREGISTERED                                
	// TODO find a way to fit all 3 client types in one call
	Logger::critical("Command '" + command + "' has no viable executor");
}

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


//TODO add strerror to socket binding failed
void			IRCServer::_initStatusMessages( void )
{
	/* Errors */

	IRCServer::statusMessages[ERR_NOSUCHNICK]			=	"[target] :No such nick/channel";
	IRCServer::statusMessages[ERR_NOSUCHSERVER]			=	"[target] :No such server";
	IRCServer::statusMessages[ERR_NOSUCHCHANNEL]		=	"[target] :No such channel";
	IRCServer::statusMessages[ERR_CANNOTSENDTOCHAN]		=	"[target] :Cannot send to channel";
	IRCServer::statusMessages[ERR_TOOMANYCHANNELS]		=	"[target] :You have joined too many channels";
	IRCServer::statusMessages[ERR_WASNOSUCHNICK]		=	"[target] :There was no such nickname";
	IRCServer::statusMessages[ERR_TOOMANYTARGETS]		=	"[target] :<error code> recipients. <abort message>";
	IRCServer::statusMessages[ERR_NOSUCHSERVICE]		=	"[target] :No such service";
	IRCServer::statusMessages[ERR_NOORIGIN]				=	":No origin specified";//NO TARGET
	IRCServer::statusMessages[ERR_NORECIPENT]			=	":No recipient given ([target])";
	IRCServer::statusMessages[ERR_NOTEXTTOSEND]			=	":No text to send";//NO TARGET
	IRCServer::statusMessages[ERR_NOTOPLEVEL]			=	"[target] :No toplevel domain specified";
	IRCServer::statusMessages[ERR_WILDTOPLEVEL]			=	"[target] :Wildcard in toplevel domaind";
	IRCServer::statusMessages[ERR_BADMASK]				=	"[target] :Bad Server/host mask";
	IRCServer::statusMessages[ERR_UNKNOWNCOMMAND]		=	"[target] :Unknown command";
	IRCServer::statusMessages[ERR_NOMOTD]				=	":MOTD File is missing";//NO TARGET
	IRCServer::statusMessages[ERR_NOADMININFO]			=	"[target] :No administrative info available";
	IRCServer::statusMessages[ERR_FILEERROR]			=	":File error doing <file op> on <file>"; // NO TARGET
	IRCServer::statusMessages[ERR_NONICKNAMEGIVEN]		=	":No nickname given"; // NO TARGET
	IRCServer::statusMessages[ERR_ERRONEUSNICKNAME]		=	"[target] :Erroneous nickname";
	IRCServer::statusMessages[ERR_NICKNAMEINUSE]		=	"[target] :Nickname is already in use";
	IRCServer::statusMessages[ERR_NICKCOLLISION]		=	"[target] :Nickname collision KILL from <user>@<host>";
	IRCServer::statusMessages[ERR_UNAVAILRESOURCE]		=	"[target] :Nick/channel is temporarily unavailable";
	IRCServer::statusMessages[ERR_USERNOTINCHANNEL]		=	"[target] [target2] :They aren't on that channel"; //2 TARGET
	IRCServer::statusMessages[ERR_NOTONCHANNEL]			=	"[target] :You're not on that channel";
	IRCServer::statusMessages[ERR_USERONCHANNEL]		=	"[target] [target2] :is already on channel"; // 2 TARGET
	IRCServer::statusMessages[ERR_NOLOGIN]				=	"[target] :User not logged in";
	IRCServer::statusMessages[ERR_SUMMONDISABLED]		=	":SUMMON has been disabled";//NO TARGET //TODO implement command
	IRCServer::statusMessages[ERR_USERSDISABLED]		=	":USERS has been disabled";//NO TARGET //TODO implement command
	IRCServer::statusMessages[ERR_NOTREGISTERED]		=	":You have not registered";//NO TARGET //TODO implement command
	IRCServer::statusMessages[ERR_NEEDMOREPARAMS]		=	"[target] :Not enough parameters";
	IRCServer::statusMessages[ERR_ALREADYREGISTRED]		=	"[target] :Unauthorized command (already registered)";
	IRCServer::statusMessages[ERR_NOPERMFORHOST]		=	":Your host isn't among the privileged"; // NO TARGET
	IRCServer::statusMessages[ERR_PASSWDMISMATCH]		=	":Password incorrect"; // NO TARGET
	IRCServer::statusMessages[ERR_YOUREBANNEDCREEP]		=	":You are banned from this server"; // NO TARGET
	IRCServer::statusMessages[ERR_YOUWILLBEBANNED]		=	""; // NO TARGET
	IRCServer::statusMessages[ERR_KEYSET]				=	"[target] :Channel key already set";
	IRCServer::statusMessages[ERR_CHANNELISFULL]		=	"[target] :Cannot join channel (+l)";
	IRCServer::statusMessages[ERR_UNKNOWNMODE]			=	"[target] :is unknown mode char to me for [target2]";//2TARGET
	IRCServer::statusMessages[ERR_INVITEONLYCHAN]		=	"[target] :Cannot join channel (+i)";
	IRCServer::statusMessages[ERR_BANNEDFROMCHAN]		=	"[target] :Cannot join channel (+b)";
	IRCServer::statusMessages[ERR_BADCHANNELKEY]		=	"[target] :Cannot join channel (+k)";
	IRCServer::statusMessages[ERR_BADCHANMASK]			=	"[target] :Bad Channel Mask";
	IRCServer::statusMessages[ERR_NOCHANMODES]			=	"[target] :Channel doesn't support modes";
	IRCServer::statusMessages[ERR_BANLISTFULL]			=	"[target] [target2] :Channel list is full"; //2 TARGET
	IRCServer::statusMessages[ERR_NOPRIVILEGES]			=	":Permission Denied- You're not an IRC operator"; //NO TARGET
	IRCServer::statusMessages[ERR_CHANOPRIVSNEEDED]		=	"[target] :You're not channel operator";
	IRCServer::statusMessages[ERR_CANTKILLSERVER]		=	":You can't kill a server!"; //NO TARGET //TODO implment it
	IRCServer::statusMessages[ERR_RESTRICTED]			=	":Your connection is restricted!"; //NO TARGET
	IRCServer::statusMessages[ERR_UNIQOPPRIVSNEEDED]	=	":You're not the original channel operator"; //NO TARGET
	IRCServer::statusMessages[ERR_NOOPERHOST]			=	"[target] :No O-lines for your host";
	IRCServer::statusMessages[ERR_UMODEUNKNOWNFLAG]		=	"[target] :Unknown MODE flag";
	IRCServer::statusMessages[ERR_USERSDONTMATCH]		=	"[target] :Cannot change mode for other users";
}


/* ************************************************************************** */