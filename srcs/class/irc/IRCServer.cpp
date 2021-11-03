#include "IRCServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
//REVIEW Server name maximum 63 character

IRCServer::IRCServer(int port, const std::string & password, const std::string &host) : AServer(this->_protocol, host, port), _protocol(IRCProtocol()), _forwardSocket(this->_protocol), _password(password), _networkSocket("")
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
	std::cout << "[IRC] Client " << s.getSocket() << " joined the server !" << std::endl;
	Package pack = Package(this->_protocol, std::string("<") + std::to_string(s.getSocket()) + "> joined the server !\r\n");
	this->sendAll(pack, &s);
	//TODO delete package? 
}

void							IRCServer::_onClientRecv(SockStream &s, Package &pkg)
{
	AClient& c = getClientBySockStream(s);
	std::cout << "[IRC]<" << c.getStream().getSocket() << ">" << pkg.getRawData();
	Package pack = Package(this->_protocol, std::string("<") + std::to_string(c.getStream().getSocket()) + "> " + pkg.getRawData());
	this->execute(c, pkg.getData());
	this->sendAll(pack, &s);
	//TODO delete package? 
}

void							IRCServer::_onClientQuit(SockStream &s)
{
	std::cout << "[IRC] Client " << s.getSocket() << " disconnected." << std::endl;

	Package pack = Package(this->_protocol, std::string("<") + std::to_string(s.getSocket()) + "> disconnected.\r\n"); // TODO why trailing \r\n?
	//TODO remove client from lists
	this->sendAll(pack, &s);
	//TODO delete package? 
}

void		IRCServer::setRegistered(AClient & client)
{
	client.setRegistered(true);
	this->_ircClients.insert(make_pair(client.getNickname(), &client));
	this->_pendingConnections.erase(std::find(this->_pendingConnections.begin(), this->_pendingConnections.end(), &client));
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
	this->_userCommands.insert(std::make_pair("USER", &IRCServer::userCommandUser));
	this->_userCommands.insert(std::make_pair("PASS", &IRCServer::userCommandPass));
	this->_userCommands.insert(std::make_pair("NICK", &IRCServer::userCommandNick));
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
				//REVIEW to_string is c++11
				uint ret = (this->*(this->_userCommands[command]))(dynamic_cast<Client&>(client), args);
				this->sendPackage(new Package(this->_protocol, std::to_string(ret) + "\r\n"), client.getStream());
				// std::cout << "Return: " << (this->*(this->_userCommands[command]))(dynamic_cast<Client&>(client), args) << std::endl;
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
		return ERR_KICK; // kickUser
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
	std::cout << "<" << client.getStream().getSocket() << "> Command <USER> with args: " << cmd << std::endl;
	if (client.isRegistered())
		return ERR_ALREADYREGISTRED;
	else if (Parser::nbParam(cmd) < 4)
		return ERR_NEEDMOREPARAMS;
	client.setUsername(Parser::getParam(cmd, 0));
	client.setDomaineName(Parser::getParam(cmd, 1));
	client.setServername(Parser::getParam(cmd, 2));
	client.setRealname(Parser::getParam(cmd, 3));
	this->setRegistered(client);
	return SUCCESS;
}

uint		IRCServer::serverCommandNick(Client & client, std::string cmd)
{
	std::cout << "<" << client.getStream().getSocket() << "> Command <NICK> with args: " << cmd << std::endl;
	if (client.getPassword().empty())
		return false;
	return SUCCESS;
}

/* ************************************************************************** */