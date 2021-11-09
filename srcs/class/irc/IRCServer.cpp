#include "IRCServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
//REVIEW Server name maximum 63 character
IRCServer::IRCServer(ushort port, const std::string & password, const std::string &host)
: ASockManager(), ANode(host), _forwardSocket(0), _password(password), _protocol()
{
	Logger::debug("IRCServer constructor");
	Logger::info("IRCServer host: " );
	Logger::info("IRCServer host: " + ntos(host));
	Logger::info("IRCServer port: " + ntos(port));
	Logger::info("IRCServer password :" + ntos(password));
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
	Logger::info("- host		: " + ntos(host)); 
	Logger::info("- port		: " + ntos(port)); 
	Logger::info("- password	: " + ntos(password));
	if (this->connectOn(host, port, this->_protocol) == false)
	{
		Logger::error("Cannot connect to network");
		return false;
	}
	//TODO work here
	//TODO handle here a real exchange with server
	// this->sendPackage(new Package(this->_protocol, this->_protocol.format("message\r\n")), this->_socket[this->_forwardSocket]);
	return true;
}

void							IRCServer::_setRegistered(Client & client)
{
	client.setRegistered(true);
	this->_ircClients.insert(make_pair(client.getNickname(), &client));
	this->_pendingConnections.remove(&client);
}

void							IRCServer::_sendMessage(AEntity & target, std::string message, SockStream * except)
{
	//TODO add server prefix
	if (target.getType() == AEntity::value_type_channel)
	{
		Logger::debug("Sending channle message");
		Package pkg = Package(this->_protocol, this->_protocol.format(message));
		reinterpret_cast<Channel*>(&target)->broadcastPackage(pkg, except);
	}
	else
	{
		Logger::debug("Sending Client message");
		Package *package = new Package(this->_protocol, this->_protocol.format(message), &reinterpret_cast<Client*>(&target)->getStream());
		this->sendPackage(package, reinterpret_cast<Client*>(&target)->getStream());
	}
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
	Logger::debug("Server receive: " + pkg.getData());
	if (! pkg.getData().empty())
		this->execute(*c, pkg.getData());
}



void							IRCServer::_onClientQuit(SockStream &s)
{
	Package pack = Package(this->_protocol, std::string("<") + ntos(s.getSocket()) + "> disconnected.\r\n"); // REVIEW why trailing \r\n?
	Client* cli = this->getClientBySockStream(s);
	if (cli == NULL)
	{
		Logger::critical("SockStream without Client associated !!! " + ntos(s.getSocket()));
		return ;
	}
	if (cli->isRegistered())
	{
		Logger::warning("Client " + cli->getNickname() + " disconnected.");
		std::string nick = cli->getNickname();
		delete this->_ircClients[nick];
		this->_ircClients.erase(nick);
	}
	else
	{
		Logger::warning("unknown client " + ntos(s.getSocket()) + " disconnected.");
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
		if ((it->second->getType() != AEntity::value_type_channel)
			&& reinterpret_cast<Client *>(it->second)->getStream().getSocket() == s.getSocket())
			return reinterpret_cast<Client *>(it->second);
	}
	//get in  pending list
	std::list<Client*>::iterator it;
	for ( it = this->_pendingConnections.begin(); it != this->_pendingConnections.end(); ++it)
	{
		if ((*it)->getStream().getSocket() == s.getSocket())
			return *it;
	}
	return NULL;
}

const IProtocol&					IRCServer::getProtocol( void ) const
{
	return this->_protocol;
}

/*
** --------------------------------- COMMANDS ---------------------------------
*/

void				IRCServer::_init_commands( void )
{
	this->_userCommands.insert(std::make_pair("USER",	&IRCServer::_commandUSER));
	this->_userCommands.insert(std::make_pair("PASS",	&IRCServer::_commandPASS));
	this->_userCommands.insert(std::make_pair("NICK",	&IRCServer::_commandNICK));
	this->_userCommands.insert(std::make_pair("PRIVMSG",	&IRCServer::_commandPRIVMSG));
	this->_userCommands.insert(std::make_pair("DESCRIBE",	&IRCServer::_commandDESCRIBE));
	this->_userCommands.insert(std::make_pair("JOIN",	&IRCServer::_commandJOIN));
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
					//TODO
					std::string prefix = ":" + client.getNickname() + "!server-ident@sender-server ";
					this->_sendMessage(client, prefix + ntos(ret) + std::string(" ") + client.getNickname() + " :No such nick/channel"); // senError
				}
			}
			else
				Logger::warning(client.getNickname() + ": unknown command");
			break;
		case Client::value_type_server:
			Logger::info("remote Server command");
			if (this->_serverCommands.count(command) == 1)
				(this->*(this->_serverCommands[command]))(dynamic_cast<Client&>(client), args);
			else
				Logger::warning("unknown command");
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
	this->_setRegistered(client);
	Logger::info("new user registered: " + client.getNickname());
	return SUCCESS;
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
	std::string msg = ":" + client.getNickname() + "!server-ident@sender-server PRIVMSG " + target->getNickname() + " :" + Parser::getParam(cmd, 1);
	Logger::debug("to " + client.getNickname() + " send " + msg);
	if (target->getType() == AEntity::value_type_channel)
		this->_sendMessage(*target, msg, &client.getStream());
	else
		this->_sendMessage(*target, msg);
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
//TODO add paramToList to every needed

uint		IRCServer::_commandJOIN(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<JOIN> with args: " + cmd );
	size_t nbParam = Parser::nbParam(cmd);
	if ( nbParam == 0)
		return ERR_NEEDMOREPARAMS;
	// this->_sendMessage(client, client.getNickname() + cmd);
	std::list<std::string> channels(Parser::paramToList(Parser::getParam(cmd, 0)));
	if (channels.size() == 1 && channels.front().compare("0") == 0)
		return client.leaveAllChannels();
	if (client.isFull())
		return ERR_TOOMANYCHANNELS;
	std::list<std::string> keys(Parser::paramToList(Parser::getParam(cmd, 1)));

	std::list<std::string>::iterator itk = keys.begin();
	for (std::list<std::string>::iterator itc = channels.begin(); itc != channels.end(); ++itc)
	{
		if (itk != keys.end())
			Logger::debug("client " + client.getNickname() + ": ask to join channel: " + *itc + " with key: " + *itk++);
		else
			Logger::debug("client " + client.getNickname() + ": ask to join channel: " + *itc);
		if (this->_ircClients.count(*itc) == false)
		{
			//Channel don't exist
			if ( Parser::validChannelName(*itc) == false)
				return ERR_NOSUCHCHANNEL;
			Logger::info("Creating a new channel: " + *itc);
			Channel* new_chan =  new Channel(*itc);
			new_chan->addClient(client);
			this->_ircClients.insert(std::make_pair(*itc, new_chan));
			//TODO send JOIN as success
			return SUCCESS;
		}
		AEntity* aChannel = this->_ircClients.find(*itc)->second;
		if (aChannel->getType() != AEntity::value_type_channel)
			return ERR_NOSUCHCHANNEL;
		else
		{
			Logger::info("Adding " + client.getNickname() + " on channel: " + *itc);
			return dynamic_cast<Channel*>(aChannel)->addClient(client);
		}
	}
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
		if (it->second->getType() == Channel::value_type_channel)
			Logger::debug("Channel K:" + it->first + " / N: " + it->second->getNickname() + " / P: " + it->second->getPassword());
		else
		{
			Client * cli = reinterpret_cast<Client *>(it->second);
			Logger::debug("   - K:" + it->first + " / N: " + it->second->getNickname() + " / P: " + cli->getPassword() + " / R: " + ntos(cli->isRegistered()));
		}
	}
	
}

/* ************************************************************************** */