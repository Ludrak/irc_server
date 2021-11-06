#include "IRCServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
//REVIEW Server name maximum 63 character
//REVIEW to_string is c++11 (view in all project)

IRCServer::IRCServer(int port, const std::string & password, const std::string &host) : AServer(*(new IRCProtocol()), host, port), _forwardSocket(*(this->_protocol)), _password(password), _networkSocket("")
{
	std::cout << "IRCServer constructor" << "\n";
	std::cout << "IRCServer host:" << host << "\n";
	std::cout << "IRCServer port:" << port << "\n";
	std::cout << "IRCServer password:" << password << "\n" << std::endl;
	this->_init_commands();
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

IRCServer::~IRCServer()
{
	// delete all auth clients
	for (std::map<std::string, AIrcClient *>::iterator it = this->_ircClients.begin(); it != this->_ircClients.end(); ++it)
	{
		delete (*it).second;
	}
	// delete all non-auth clients
	std::list<AIrcClient *>::iterator it;
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
	std::cout << "Network" << std::endl; 
	std::cout << "host		- " << host << std::endl; 
	std::cout << "port		- " << port << std::endl; 
	std::cout << "password	- " << password << std::endl;
	return true;
}

/*
** --------------------------------- EVENTS ----------------------------------
*/


void						IRCServer::_onClientJoin(SockStream &s)
{
	this->_pendingConnections.push_back(new Client(*this, s));
	Logger::log(INFO, "New connection: socket<" + std::to_string(s.getSocket()) + "> joined the server !");
}

void							IRCServer::_onClientRecv(SockStream &s, Package &pkg)
{
	AIrcClient& c = getClientBySockStream(s);
	this->printServerState();
	Logger::log(DEBUG, "Server receive: " + pkg.getData());
	this->execute(c, pkg.getData());
}

void							IRCServer::_onClientQuit(SockStream &s)
{
	Package pack = Package(*(this->_protocol), std::string("<") + std::to_string(s.getSocket()) + "> disconnected.\r\n"); // TODO why trailing \r\n?
	AIrcClient & cli = this->getClientBySockStream(s);
	if (cli.isRegistered())
	{
		Logger::log(WARNING, "Client " + cli.getNickname() + " disconnected.");
		std::string nick = cli.getNickname();
		delete this->_ircClients[nick];
		this->_ircClients.erase(nick);
	}
	else
	{
		Logger::log(WARNING, "unknow client " + std::to_string(s.getSocket()) + " disconnected.");
		delete &cli;
		this->_pendingConnections.remove(&cli);
	}
}

void							IRCServer::setRegistered(AIrcClient & client)
{
	client.setRegistered(true);
	this->_ircClients.insert(make_pair(client.getNickname(), &client));
	this->_pendingConnections.remove(&client);
}

void							IRCServer::sendMessage(AIrcClient & client, std::string message, uint error)
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

AIrcClient&							IRCServer::getClientBySockStream(SockStream & s)
{
	//get in map of user
	for (std::map<std::string, AIrcClient*>::iterator it = this->_ircClients.begin(); it != this->_ircClients.end(); ++it)
	{
		if (&(*it).second->getStream() == &s)
			return *(*it).second;
	}
	//get in  pending list
	std::list<AIrcClient*>::iterator it;
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

int					IRCServer::execute(AIrcClient & client, std::string data)
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
					this->sendMessage(client, std::to_string(ret), true);
			}
			else
				std::cerr << "unknow command" << std::endl;
			break;
		case 2:
			std::cout << "Server command" << std::endl;
			if (this->_serverCommands.count(command) == 1)
				(this->*(this->_serverCommands[command]))(dynamic_cast<Client&>(client), args);
			else
				std::cerr << "unknow command" << std::endl;
			break;
		default:
			std::cerr << "unknow client type" << std::endl;
			break;
	}
	return 1;
}

uint		IRCServer::userCommandPass(Client & client, std::string cmd)
{
	std::cout << "<" << client.getStream().getSocket() << "> Command <PASS> with args: " << cmd << std::endl;
	if (client.isRegistered())
		return ERR_ALREADYREGISTRED;
	else if (cmd.empty())
		return ERR_NEEDMOREPARAMS;
	client.setPassword(cmd);
	return SUCCESS;
}

uint		IRCServer::userCommandNick(Client & client, std::string cmd)
{
	std::cout << "<" << client.getStream().getSocket() << "> Command <NICK> with args: " << cmd << std::endl;
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
	std::cout << "<" << client.getStream().getSocket() << ">  with args: " << cmd << std::endl;
	if (client.isRegistered())
		return ERR_ALREADYREGISTRED;
	else if (Parser::nbParam(cmd) < 4)
		return ERR_NEEDMOREPARAMS;
	client.setUsername(Parser::getParam(cmd, 0));
	client.setDomaineName(Parser::getParam(cmd, 1));
	client.setServername(Parser::getParam(cmd, 2));
	client.setRealname(Parser::getParam(cmd, 3));
	this->setRegistered(client);
	Logger::log(INFO, "new user registered: " + client.getNickname());
	return SUCCESS;
}
//TODO Parser::get param => error including on space
//TODO implement channels here and list of nickname/channels
uint		IRCServer::userCommandPrivmsg(Client & client, std::string cmd)
{
	std::cout << "<" << client.getStream().getSocket() << "> Command <PRIVMSG> with args: " << cmd << std::endl;
	Logger::log(INFO, client.getNickname() + "<PRIVMSG>: " + cmd);
	size_t nbParam = Parser::nbParam(cmd);
	Logger::log(DEBUG, "nbParam = " + std::to_string(nbParam));
	if (nbParam == 0)
		return ERR_NORECIPENT;
	else if (nbParam == 1)
		return ERR_NOTEXTTOSEND;
	else if (nbParam > 2)
		return SUCCESS; //REVIEW I don't know what to do here
	std::string target_name = Parser::getParam(cmd, 0); 
	if (this->_ircClients.count(target_name) == 0)
		return ERR_NOSUCHNICK;
	AIrcClient *target = this->_ircClients[target_name];
	Logger::log(DEBUG, "target_nick = " + target->getNickname());
	Logger::log(DEBUG, "text = " + Parser::getParam(cmd, 1));
	std::string msg = ":" + client.getNickname() + "!server-ident@sender-server PRIVMSG " + target->getNickname() + " :" + Parser::getParam(cmd, 1);
	this->sendMessage(*target, msg);
	return SUCCESS;
}

uint		IRCServer::userCommandDescribe(Client & client, std::string cmd)
{
	std::cout << "<" << client.getStream().getSocket() << "> Command <DESCRIBE> with args: " << cmd << std::endl;
	if (Parser::nbParam(cmd) != 2)
	this->sendMessage(client, client.getNickname() + cmd);
	return SUCCESS;
}

uint		IRCServer::serverCommandNick(Client & client, std::string cmd)
{
	std::cout << "<" << client.getStream().getSocket() << "> Command <NICK> with args: " << cmd << std::endl;
	if (client.getPassword().empty())
		return false;
	return SUCCESS;
}

/* ************************************DEBUG********************************** */

void			IRCServer::printServerState( void )
{
	Logger::log(DEBUG, "Server : p: " + std::to_string(this->_pendingConnections.size()) + ", r: " + std::to_string(this->_ircClients.size()) );
	Logger::log(DEBUG, "pending:");
	for (std::list<AIrcClient *>::iterator it = this->_pendingConnections.begin(); it != this->_pendingConnections.end(); ++it)
		Logger::log(DEBUG, "   - N: " + (*it)->getNickname() + " / P: " + (*it)->getPassword() + " / R: " + std::to_string((*it)->isRegistered()));
	Logger::log(DEBUG, "registered:");
	for (std::map<std::string, AIrcClient *>::iterator it = this->_ircClients.begin(); it != this->_ircClients.end(); ++it)
		Logger::log(DEBUG, "   - K:" + (*it).first + " / N: " + (*it).second->getNickname() + " / P: " + (*it).second->getPassword() + " / R: " + std::to_string((*it).second->isRegistered()));

}

/* ************************************************************************** */