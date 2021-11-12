#include "IRCServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
//REVIEW Server name maximum 63 character
IRCServer::IRCServer(ushort port, const std::string & password, const std::string &host)
: ASockManager(), ANode(host), _forwardSocket(0), _password(password), _protocol()
{
	Logger::debug("IRCServer constructor");
	Logger::info("IRC Server:");
	Logger::info("- host     : " + ntos(host));
	Logger::info("- port     : " + ntos(port));
	Logger::info("- password :" + ntos(password));
	this->_init_commands();
	this->listenOn(port, this->_protocol);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

IRCServer::~IRCServer()
{
	// delete all auth clients
	for (std::map<std::string, AEntity *>::iterator it = this->_ircClients.begin(); it != this->_ircClients.end(); ++it)
	{
		delete (*it).second;
	}
	// delete all non-auth clients
	std::list<Client *>::iterator it;
	for ( it = this->_pendingConnections.begin(); it != this->_pendingConnections.end(); ++it)
	{
		delete (*it);
	}
}

/*
** --------------------------------- METHODS ----------------------------------
*/

bool						IRCServer::setNetworkConnection(const std::string & host, ushort port, std::string & password)
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
		// connection exception, we are root on that server
		Logger::info ("Forward connection failed: running server as root node");
		return false;
	}
	return true;
}

void							IRCServer::_setRegistered(Client & client, int type)
{
	if (type == Client::value_type_client)
	{
		client.setRegisteredAsClient(true);
		this->_ircClients.insert(make_pair(client.getNickname(), &client));
		this->_pendingConnections.remove(&client);
	}
	else if (type == Client::value_type_server)
	{
		client.setRegisteredAsServer(true);
		this->_ircClients.insert(make_pair(client.getSID(), &client));
		this->_pendingConnections.remove(&client);
	}
}


void							IRCServer::_sendMessage(AEntity & target, const std::stringstream &message)
{
	//TODO add server prefix
	if (target.getType() == Channel::value_type)
	{
		Logger::debug("Sending channel message");
		Package *package = new Package(this->_protocol, this->_protocol.format(message.str()));
		reinterpret_cast<Channel*>(&target)->broadcastPackage(package);
	}
	else
	{
		Logger::debug("Sending Client message");
		Package *package = new Package(this->_protocol, this->_protocol.format(message.str()), &reinterpret_cast<Client*>(&target)->getStream());
		this->sendPackage(package, reinterpret_cast<Client*>(&target)->getStream());
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

void						IRCServer::_onClientJoin(SockStream & s)
{
	this->_pendingConnections.push_back(new Client(s));
	Logger::info("New connection: socket<" + ntos(s.getSocket()) + "> joined the server !");
	this->_printServerState();
}




void							IRCServer::_onClientRecv(SockStream & s, Package & pkg)
{
	Client* c = getClientBySockStream(s);
	if (c == NULL)
	{
		this->_printServerState();
		Logger::critical("SockStream without Client associated !!! " + ntos(s.getSocket()));
		return ;
	}
	Logger::debug("recv [" + s.getIP() + "] : " + pkg.getData());
	this->execute(*c, pkg.getData());
}



void							IRCServer::_onClientQuit(SockStream &s)
{
	Client* cli = this->getClientBySockStream(s);
	if (cli == NULL)
	{
		Logger::critical("SockStream without Client associated !!! " + ntos(s.getSocket()));
		return ;
	}
	if (cli->isRegistered())
	{
		Logger::warning("Client " + cli->getNickname() + " disconnected.");
		if (cli->getType() == Client::value_type_server)
		{
			/* for server SID is used as unique identifier */
			/* disconnect all servers correspondig to that socket since those servers are now netsplitted from the network */
			for (std::map<std::string, AEntity *>::iterator it = this->_ircClients.begin(); it != this->_ircClients.end();)
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
		Logger::warning("unknow client " + ntos(s.getSocket()) + " disconnected.");
		delete cli;
		this->_pendingConnections.remove(cli);
	}
}




void						IRCServer::_onRecv( SockStream &server,  Package &pkg)
{
	Logger::debug("Receiving package from forward server");
	Logger::debug("data: " + pkg.getData() );
	if (server.getSocket() != this->_forwardSocket)
	{
		Logger::error("Receving response from a server that's not the forward server.");
		return ;
	}
	
}




//Called when we connect to
void				IRCServer::_onConnect ( SockStream &server)
{
	Logger::info("Connecting to forward server");
	this->_forwardSocket = server.getSocket();
	std::stringstream ss;
	ss << "SERVER " << this->_name << " 0 " << this->_token << " :" << this->_info; 
	this->_sendMessage(server, ss);
}
		
void				IRCServer::_onQuit( SockStream &server)
{
	Logger::warning("Stop connection to forward server");
	if (server.getSocket() != this->_forwardSocket)
	{
		Logger::error("Stopping connection with a remote server that's not the forward server.");
		return ;
	}
	this->_forwardSocket = 0;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

Client*							IRCServer::getClientBySockStream(SockStream & s)
{
	//get in map of user
	for (std::map<std::string, AEntity*>::iterator it = this->_ircClients.begin(); it != this->_ircClients.end(); ++it)
	{
		if (reinterpret_cast<Client *>(it->second)->getStream().getIP() == s.getIP())
			return reinterpret_cast<Client *>(it->second);
	}
	//get in  pending list
	std::list<Client*>::iterator it;
	for ( it = this->_pendingConnections.begin(); it != this->_pendingConnections.end(); ++it)
	{
		if ((*it)->getStream().getIP() == s.getIP())
			return *it;
	}
	return NULL;
}

const IProtocol&					IRCServer::getProtocol( void ) const
{
	return this->_protocol;
}

const std::string		&IRCServer::getName() const
{
	return (this->_name);
}

const std::string		&IRCServer::getInfo() const
{
	return (this->_info);
}

uint					IRCServer::getToken() const
{
	return (this->_token);
}

void					IRCServer::setName(const std::string &name)
{
	this->_name = name;
}

void					IRCServer::setInfo(const std::string &info)
{
	this->_info = info;	
}

void					IRCServer::setToken(const uint token)
{
	this->_token = token;
}


/*
** --------------------------------- COMMANDS ---------------------------------
*/

void				IRCServer::_init_commands( void )
{
	this->_unregisteredCommands.insert(std::make_pair("USER",	&IRCServer::_commandUSER));
	this->_unregisteredCommands.insert(std::make_pair("PASS",	&IRCServer::_commandPASS));
	this->_unregisteredCommands.insert(std::make_pair("NICK",	&IRCServer::_commandNICK));
	this->_unregisteredCommands.insert(std::make_pair("SERVER",	&IRCServer::_commandSERVER));

	this->_userCommands.insert(std::make_pair("USER",	&IRCServer::_commandUSER));
	this->_userCommands.insert(std::make_pair("PASS",	&IRCServer::_commandPASS));
	this->_userCommands.insert(std::make_pair("NICK",	&IRCServer::_commandNICK));
	this->_userCommands.insert(std::make_pair("PRIVMSG",	&IRCServer::_commandPRIVMSG));
	this->_userCommands.insert(std::make_pair("DESCRIBE",	&IRCServer::_commandDESCRIBE));

	this->_serverCommands.insert(std::make_pair("SERVER",	&IRCServer::_commandSERVER));
}

int					IRCServer::execute(AEntity & client, std::string data)
{
	size_t sep = data.find(" ");
	std::string command(data);
	std::string args("");
	if (sep < data.size() - 1)
	{
		command = data.substr(0, sep);
		args = data.substr(sep + 1);
	}
	switch (client.getType())
	{
		case Client::value_type_client:
			if (this->_userCommands.count(command) == 1)
			{
				uint ret = (this->*(this->_userCommands[command]))(dynamic_cast<Client&>(client), args);
				if (ret != 0)
				{
					std::string prefix = ":" + client.getNickname() + "!server-ident@sender-server ";
					std::stringstream ss;
					ss << prefix << ret;
					this->_sendMessage(client, ss); // senError
				}
			}
			else
				Logger::warning(client.getNickname() + ": unknown command");
			break;
		case Client::value_type_server:
			if (this->_serverCommands.count(command) == 1)
			{
				uint ret = (this->*(this->_serverCommands[command]))(dynamic_cast<Client&>(client), args);
				if (ret != 0)
				{
					std::string prefix = ":" + client.getNickname() + "!server-ident@sender-server ";
					std::stringstream ss;
					ss << prefix << ret;
					this->_sendMessage(client, ss); // senError
				}
			}
			else
				Logger::warning("unknown command");
			break;
		case Client::value_type_unknown:
			Logger::debug("yet unidentified client");
			if (this->_unregisteredCommands.count(command) == 1)
			{
				uint ret = (this->*(this->_unregisteredCommands[command]))(dynamic_cast<Client&>(client), args);
				if (ret != 0)
				{
					std::string prefix = ":" + client.getNickname() + "!server-ident@sender-server ";
					std::stringstream ss;
					ss << prefix << ret;
					this->_sendMessage(client, ss); // senError
				}
			}
			else
				Logger::warning(client.getNickname() + ": unknown command");
			break;
		default:
			Logger::error("unknown client type");
			break;
	}
	return 1;
}

uint		IRCServer::_commandPASS(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<PASS> with args: " + cmd );
	if (client.isRegistered())
		return ERR_ALREADYREGISTRED;
	else if (cmd.empty())
		return ERR_NEEDMOREPARAMS;
	client.setPassword(cmd);
	return SUCCESS;
}

uint		IRCServer::_commandNICK(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<NICK> with args: " + cmd );
	std::string nick = Parser::getParam(cmd, 0);
	if (client.getPassword() != this->_password)
	{
		//TODO handle this error without disconnecting
		this->disconnect(client.getStream()); // kickUser
		return SUCCESS;
	}
	else if (Parser::nbParam(cmd) != 1) //REVIEW normalement si 2 param venant d'un user, commande ignoré
		return ERR_NONICKNAMEGIVEN;
	else if (Parser::validNickname(nick) == false)
		return ERR_ERRONEUSNICKNAME;
	if (this->_ircClients.count(nick) != 0)
		return ERR_NICKNAMEINUSE;
	if (client.isRegistered()
	&& this->_ircClients.count(client.getNickname()) != 0)
	{
		this->_ircClients.erase(client.getNickname());
		this->_ircClients.insert(std::make_pair(nick, &client));
	}
	client.setNickname(nick);
	return SUCCESS;
}

uint		IRCServer::_commandUSER(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<USER> with args: " + cmd );
	if (client.isRegistered())
		return ERR_ALREADYREGISTRED;
	else if (Parser::nbParam(cmd) < 4)
		return ERR_NEEDMOREPARAMS;
	client.setUsername(Parser::getParam(cmd, 0));
	client.setDomaineName(Parser::getParam(cmd, 1));
	client.setServername(Parser::getParam(cmd, 2));
	client.setRealname(Parser::getParam(cmd, 3));
	this->_setRegistered(client, Client::value_type_client);
	Logger::info("new user registered: " + client.getNickname());
	return SUCCESS;
}


uint		IRCServer::_commandSERVER(Client & client, std::string cmd)
{
	//TODO see what to do when no param specified
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<SERVER> with args: " + cmd );
	if (Parser::nbParam(cmd) < 4)
		return SUCCESS;
	std::string	serverName = Parser::getParam(cmd, 0);
	int			hopCount = std::stoi(Parser::getParam(cmd, 1));
	std::string	info = Parser::getParam(cmd, 3);
	
	uint token = 0;
	try
	{
		token = std::stoi(Parser::getParam(cmd, 2));
		if (token > 999)
			throw ;
	}
	catch(const std::exception& e)
	{
		// TODO invalid token ?
		Logger::error ("Server trying to connect has invalid token: \"" + Parser::getParam(cmd, 2) + "\"");
		return (SUCCESS);
	}

	if (token == this->_token || this->_ircClients.count(std::to_string (token)) == 1)
	{
		Logger::error("already regristered server ! (" + client.getStream().getIP() + ")");
		Logger::error("closing connection to " + client.getStream().getIP() + " to preserve network stability.");
		//TODO this->disconnect(client.getStream());
		return ERR_ALREADYREGISTRED;
	}
	if (client.getType() == Client::value_type_unknown)
	{
		if (hopCount != 0)
		{
			/* cannot introduce new server behind while not regristered */
			Logger::error ("unregistered client tries to introduce new server behind himself");
			return (SUCCESS); // ?
		}
		else
		{
			client.setNickname(serverName);
			client.setHopCount(hopCount);
			client.setSID(std::to_string(token));
			client.setServerDescription(info);
			this->_setRegistered(client, Client::value_type_server);
			this->_printServerState();
			Logger::info("new server connected to network: " + serverName + " (" + client.getSID() + "@" + client.getStream().getIP() + ")");
		}
	}
	else if (hopCount == 0)
	{

		/* server trying to register twice */
		Logger::error("Server " + client.getSID() + "@" + client.getStream().getIP() + " tried to register twice");
		return (SUCCESS); // ?
	}
	else if (hopCount > 0)
	{
		/* 
		** if server_reference was registered by client we need to pass it a reference 
		** to it's sockstream so that we can communicate with it later easily
		*/ 
		Client *server_reference = new Client(client.getStream());
		server_reference->setNickname(serverName);
		server_reference->setHopCount(hopCount);
		server_reference->setSID(std::to_string(token));
		server_reference->setServerDescription(info);
		server_reference->setRelayed(true);
		this->_setRegistered(*server_reference, Client::value_type_server);
		this->_printServerState();
		Logger::info("new server connected to sub network: " + serverName + " (" + server_reference->getSID() + "@" + server_reference->getStream().getIP() + " is " + ntos(hopCount) + " hop(s) away)");
	}

	// relay to forward and all connected servers
	// meantime relay all infos about our connections to new server
	std::stringstream relay_all_ss;
	relay_all_ss << "SERVER " << serverName << " " << (hopCount + 1) << " " << token << " :" << info;
	if (this->_forwardSocket != 0)
	{
		Logger::debug("relaying server connection to forward server (" + this->_sockets[this->_forwardSocket]->getIP() + ")");
		this->_sendMessage(*this->_sockets[this->_forwardSocket], relay_all_ss);
		//relay_new_ss << "SERVER " <<  << " " << (hopCount + 1) << " " << token << " :" << info;
		// TODO SEND / RECV FORWARD INFORMATIONS
	}
	for (std::map<std::string, AEntity *>::iterator it = this->_ircClients.begin(); it != this->_ircClients.end(); it++)
	{
		if (it->second->getType() == Client::value_type_server && reinterpret_cast<Client*>(it->second)->getSID() != client.getSID())
		{
			Client *srv = reinterpret_cast<Client*>(it->second);
			Logger::debug("relaying server connection to connected server (" + srv->getStream().getIP() + ")");
			this->_sendMessage(*it->second, relay_all_ss);
			// TODO how to implement multiple backward relays
			
			//std::stringstream relay_new_ss;
			//relay_new_ss << "SERVER " << srv->getNickname() << " " << srv->getHopCount() + 1 << " " << srv->getSID() << " :" << srv->getServerDescription();
			//this->_sendMessage(*server_reference, relay_new_ss);
		}
	}
	return (SUCCESS);
}



//TODO Parser::get param => error including on space
//TODO implement channels here and list of nickname/channels
uint		IRCServer::_commandPRIVMSG(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<PRIVMSG> with args: " + cmd );
	Logger::info(client.getNickname() + "<PRIVMSG>: " + cmd);
	size_t nbParam = Parser::nbParam(cmd);
	Logger::debug("nb param = " + ntos(nbParam)); 
	if (nbParam == 0)
		return ERR_NORECIPENT;
	else if (nbParam == 1)
		return ERR_NOTEXTTOSEND;
	else if (nbParam > 2)
		return SUCCESS; //REVIEW I don't know what to do here
	std::string target_name = Parser::getParam(cmd, 0);
	Logger::debug("Param target = " + target_name); 
	if (this->_ircClients.count(target_name) == 0)
	{
		Logger::debug("Target not found"); 
		return ERR_NOSUCHNICK;
	}
	AEntity *target = this->_ircClients[target_name];
	std::string prefix = "";
	std::stringstream ss;
	ss << ":" << client.getNickname() << "!server-ident@sender-server PRIVMSG " << target->getNickname() << " :" << Parser::getParam(cmd, 1);
	Logger::debug("to " + client.getNickname() + " send " + ss.str());
	this->_sendMessage(*target, ss);
	return SUCCESS;
}

uint		IRCServer::_commandDESCRIBE(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<DESCRIBE> with args: " + cmd );
	// if (Parser::nbParam(cmd) != 2)
		// this->_sendMessage(client, client.getNickname() + cmd);
		//TODO implement it
	return SUCCESS;
}

/* ************************************DEBUG********************************** */

void			IRCServer::_printServerState( void )
{
	Logger::debug("Server : p: " + ntos(this->_pendingConnections.size()) + ", r: " + ntos(this->_ircClients.size()) );
	Logger::debug("pending:");
	for (std::list<Client *>::iterator it = this->_pendingConnections.begin(); it != this->_pendingConnections.end(); ++it)
		Logger::debug("   - N: " + (*it)->getNickname() + " / P: " + (*it)->getPassword() + " / R: " + ntos((*it)->isRegistered()) + " / S: " + ntos((*it)->getStream().getSocket()));
	Logger::debug("registered:");
	for (std::map<std::string, AEntity *>::iterator it = this->_ircClients.begin(); it != this->_ircClients.end(); ++it)
	{
		if (it->second->getType() == Channel::value_type)
			Logger::debug("Channel K:" + it->first + " / N: " + it->second->getNickname() + " / P: " + it->second->getPassword());
		else
		{
			Client * cli = reinterpret_cast<Client *>(it->second);
			Logger::debug("   - K:" + it->first + " / N: " + it->second->getNickname() + " / P: " + cli->getPassword() + " / R: " + ntos(cli->isRegistered()));
		}
	}
	
}

/* ************************************************************************** */