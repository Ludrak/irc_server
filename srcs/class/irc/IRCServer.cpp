#include "IRCServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
//REVIEW Server name maximum 63 character

IRCServer::IRCServer(int port, const std::string & password, const std::string &host) : AServer(*(new IRCProtocol()), host, port), _forwardSocket(*(this->_protocol)), _password(password), _networkSocket("")
{
	Logger::debug("IRCServer constructor");
	Logger::info("IRCServer host: " + ntos(host));
	Logger::info("IRCServer port: " + ntos(port));
	Logger::info("IRCServer password :" + ntos(password));
	this->_init_commands();
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

IRCServer::~IRCServer()
{
	// delete all auth clients
	for (std::map<std::string, AClient *>::iterator it = this->_ircClients.begin(); it != this->_ircClients.end(); ++it)
	{
		delete (*it).second;
	}
	// delete all non-auth clients
	std::list<AClient *>::iterator it;
	for ( it = this->_pendingConnections.begin(); it != this->_pendingConnections.end(); ++it)
	{
		delete (*it);
	}
	delete this->_protocol;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

bool						IRCServer::setNetworkConnection(const std::string & host, int port, std::string & password)
{
	// this->_forword_socket = new SockStream(host, port);
	Logger::info("Network"); 
	Logger::info("host		- " + ntos(host)); 
	Logger::info("port		- " + ntos(port)); 
	Logger::info("password	- " + ntos(password));
	//TODO replace by setConnection given by AClient
	return true;
}

/*
** --------------------------------- EVENTS ----------------------------------
*/


void						IRCServer::_onClientJoin(SockStream &s)
{
	this->_pendingConnections.push_back(new Client(*this, s));
	Logger::info("New connection: socket<" + ntos(s.getSocket()) + "> joined the server !");
}

void							IRCServer::_onClientRecv(SockStream &s, Package &pkg)
{
	AClient& c = getClientBySockStream(s);
	this->printServerState();
	Logger::debug("Server receive: " + pkg.getData());
	this->execute(c, pkg.getData());
}

void							IRCServer::_onClientQuit(SockStream &s)
{
	Package pack = Package(*(this->_protocol), std::string("<") + ntos(s.getSocket()) + "> disconnected.\r\n"); // REVIEW why trailing \r\n?
	AClient & cli = this->getClientBySockStream(s);
	if (cli.isRegistered())
	{
		Logger::log(WARNING, "Client " + cli.getNickname() + " disconnected.");
		std::string nick = cli.getNickname();
		delete this->_ircClients[nick];
		this->_ircClients.erase(nick);
	}
	else
	{
		Logger::log(WARNING, "unknow client " + ntos(s.getSocket()) + " disconnected.");
		delete &cli;
		this->_pendingConnections.remove(&cli);
	}
}

void							IRCServer::setRegistered(AClient & client)
{
	client.setRegistered(true);
	this->_ircClients.insert(make_pair(client.getNickname(), &client));
	this->_pendingConnections.remove(&client);
}

void							IRCServer::sendMessage(AClient & client, std::string message, uint error)
{
	if (error)
	{
	//TODO add server prefix and destination suffix
		std::string prefix = ":prefix ";
		this->sendPackage(new Package(*(this->_protocol), this->_protocol->format(prefix + message + "\r\n")), client.getStream());
	}
	else
	{
		this->sendPackage(new Package(*(this->_protocol), this->_protocol->format(message + "\r\n")), client.getStream());
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const SockStream&					IRCServer::getForwardSocket( void ) const
{
	return this->_forwardSocket;
}

AClient&							IRCServer::getClientBySockStream(SockStream & s)
{
	//get in map of user
	for (std::map<std::string, AClient*>::iterator it = this->_ircClients.begin(); it != this->_ircClients.end(); ++it)
	{
		if (&(*it).second->getStream() == &s)
			return *(*it).second;
	}
	//get in  pending list
	std::list<AClient*>::iterator it;
	for ( it = this->_pendingConnections.begin(); it != --this->_pendingConnections.end(); ++it)
	{
		if (&(*it)->getStream() == &s)
			return *(*it);
	}
	return *(*it);
}

/*
** --------------------------------- COMMANDS ---------------------------------
*/

void				IRCServer::_init_commands( void )
{
	this->_userCommands.insert(std::make_pair("USER",	&IRCServer::userCommandUser));
	this->_userCommands.insert(std::make_pair("PASS",	&IRCServer::userCommandPass));
	this->_userCommands.insert(std::make_pair("NICK",	&IRCServer::userCommandNick));
	this->_userCommands.insert(std::make_pair("PRIVMSG",	&IRCServer::userCommandPrivmsg));
	this->_userCommands.insert(std::make_pair("DESCRIBE",	&IRCServer::userCommandDescribe));
}

int					IRCServer::execute(AClient & client, std::string data)
{
	size_t sep = data.find(" ");
	std::string command(data);
	std::string args("");
	if (sep < data.size() - 1)
	{
		command = data.substr(0, sep);
		args = data.substr(sep + 1);
	}
	switch (client.getId())
	{
		case Client::value_type:
			if (this->_userCommands.count(command) == 1)
			{
				uint ret = (this->*(this->_userCommands[command]))(dynamic_cast<Client&>(client), args);
				if (ret != 0)
					this->sendMessage(client, ntos(ret), true);
			}
			else
				Logger::warning(client.getNickname() + ": unknow command");
			break;
		case 2:
			Logger::info("Server command");
			if (this->_serverCommands.count(command) == 1)
				(this->*(this->_serverCommands[command]))(dynamic_cast<Client&>(client), args);
			else
				Logger::warning("unknow command");
			break;
		default:
			Logger::error("unknow client type");
			break;
	}
	return 1;
}

uint		IRCServer::userCommandPass(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<PASS> with args: " + cmd );
	if (client.isRegistered())
		return ERR_ALREADYREGISTRED;
	else if (cmd.empty())
		return ERR_NEEDMOREPARAMS;
	client.setPassword(cmd);
	return SUCCESS;
}

uint		IRCServer::userCommandNick(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<NICK> with args: " + cmd );
	std::string nick = Parser::getParam(cmd, 0);
	if (client.getPassword() != this->_password)
	{
		this->kick(client.getStream()); // kickUser
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

uint		IRCServer::userCommandUser(Client & client, std::string cmd)
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
	this->setRegistered(client);
	Logger::info("new user registered: " + client.getNickname());
	return SUCCESS;
}
//TODO Parser::get param => error including on space
//TODO implement channels here and list of nickname/channels
uint		IRCServer::userCommandPrivmsg(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<PRIVMSG> with args: " + cmd );
	Logger::info(client.getNickname() + "<PRIVMSG>: " + cmd);
	size_t nbParam = Parser::nbParam(cmd);
	if (nbParam == 0)
		return ERR_NORECIPENT;
	else if (nbParam == 1)
		return ERR_NOTEXTTOSEND;
	else if (nbParam > 2)
		return SUCCESS; //REVIEW I don't know what to do here
	std::string target_name = Parser::getParam(cmd, 0); 
	if (this->_ircClients.count(target_name) == 0)
		return ERR_NOSUCHNICK;
	AClient *target = this->_ircClients[target_name];
	std::string msg = ":" + client.getNickname() + "!server-ident@sender-server PRIVMSG " + target->getNickname() + " :" + Parser::getParam(cmd, 1);
	Logger::debug(client.getNickname() + "send " + msg);
	this->sendMessage(*target, msg);
	return SUCCESS;
}

uint		IRCServer::userCommandDescribe(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<DESCRIBE> with args: " + cmd );
	if (Parser::nbParam(cmd) != 2)
	this->sendMessage(client, client.getNickname() + cmd);
	return SUCCESS;
}

uint		IRCServer::serverCommandNick(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<NICK> with args: " + cmd );
	if (client.getPassword().empty())
		return false;
	return SUCCESS;
}

/* ************************************DEBUG********************************** */

void			IRCServer::printServerState( void )
{
	Logger::debug("Server : p: " + ntos(this->_pendingConnections.size()) + ", r: " + ntos(this->_ircClients.size()) );
	Logger::debug("pending:");
	for (std::list<AClient *>::iterator it = this->_pendingConnections.begin(); it != this->_pendingConnections.end(); ++it)
		Logger::debug("   - N: " + (*it)->getNickname() + " / P: " + (*it)->getPassword() + " / R: " + ntos((*it)->isRegistered()));
	Logger::debug("registered:");
	for (std::map<std::string, AClient *>::iterator it = this->_ircClients.begin(); it != this->_ircClients.end(); ++it)
		Logger::debug("   - K:" + (*it).first + " / N: " + (*it).second->getNickname() + " / P: " + (*it).second->getPassword() + " / R: " + ntos((*it).second->isRegistered()));

}

/* ************************************************************************** */