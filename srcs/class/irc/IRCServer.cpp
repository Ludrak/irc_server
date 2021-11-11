#include "IRCServer.hpp"

std::string				IRCServer::statusMessages[] = {};

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
//REVIEW Server name maximum 63 character
IRCServer::IRCServer(ushort port, const std::string & password, const std::string &host)
: ASockManager(), ANode(host), _forwardSocket(0), _password(password), _protocol()
{
	this->_initStatusMessages();
	this->_initCommands();
	Logger::debug("IRCServer constructor");
	Logger::info("IRCServer host: " );
	Logger::info("IRCServer host: " + ntos(host));
	Logger::info("IRCServer port: " + ntos(port));
	Logger::info("IRCServer password :" + ntos(password));
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

bool							IRCServer::setNetworkConnection(const std::string & host, ushort port, std::string & password)
{
	// this->_forword_socket = new SockStream(host, port);
	Logger::info("Try connecting to network:"); 
	Logger::info("- host	: " + ntos(host)); 
	Logger::info("- port	: " + ntos(port)); 
	Logger::info("- password: " + ntos(password));
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
	std::string prefix = ":" + client.getNickname() + "!server-ident@sender-server ";
	this->_sendMessage(client, prefix + ntos(statusCode) + " " + reply);
	return SUCCESS;
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
		Logger::debug("Sending message to channel");
		Package pkg = Package(this->_protocol, this->_protocol.format(message));
		reinterpret_cast<Channel*>(&target)->broadcastPackage(pkg, except);
	}
	else
	{
		Logger::debug("Sending message to target");
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
		cli->leaveAllChannels();
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

const IProtocol&				IRCServer::getProtocol( void ) const
{
	return this->_protocol;
}

/*
** --------------------------------- COMMANDS ---------------------------------
*/

void							IRCServer::_initCommands( void )
{
//TODO implement PART command
	this->_userCommands.insert(std::make_pair("USER",	&IRCServer::_commandUSER));
	this->_userCommands.insert(std::make_pair("PASS",	&IRCServer::_commandPASS));
	this->_userCommands.insert(std::make_pair("NICK",	&IRCServer::_commandNICK));
	this->_userCommands.insert(std::make_pair("PRIVMSG",	&IRCServer::_commandPRIVMSG));
	this->_userCommands.insert(std::make_pair("JOIN",	&IRCServer::_commandJOIN));
	this->_userCommands.insert(std::make_pair("MODE",	&IRCServer::_commandMODE));
}

int								IRCServer::execute(AEntity & client, std::string data)
{
//TODO handle prefix in data before sending data to command
	size_t sep = data.find(" ");
	std::string command(data);
	std::string args("");
	if (sep < data.size())
	{
		command = data.substr(0, sep);
		args = data.substr(sep + 1);
	}
	switch (client.getType())
	{
		case Client::value_type_client:
			if (this->_userCommands.count(command) == 1)
			{
				if ((this->*(this->_userCommands[command]))(dynamic_cast<Client&>(client), args) != SUCCESS)
					Logger::error("Error while executing command");
			}
			else
				Logger::warning("<" + client.getNickname() + ">: unknown command <" + command + ">");
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
		{
			Channel *chan = reinterpret_cast<Channel *>(it->second);
			Logger::debug("Channel K:" + it->first + " / N: " + chan->getNickname() + " / P: " + chan->getPassword());
			if (chan->getCreator() == NULL)
				Logger::debug("Channel creator: NULL");
			else
				Logger::debug("Channel creator: " + chan->getCreator()->getNickname());
			Logger::debug("Channel moderated: " + ntos(chan->isEnable(M_MODERATED)) );

		}
		else
		{
			Client * cli = reinterpret_cast<Client *>(it->second);
			Logger::debug("   - K:" + it->first + " / N: " + it->second->getNickname() + " / P: " + cli->getPassword() + " / R: " + ntos(cli->isRegistered()));
		}
	}
	
}

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