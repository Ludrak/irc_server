#include "IRCServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
//REVIEW Server name maximum 63 character
//REVIEW nickname name maximum 9 character

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
}

void							IRCServer::_onClientRecv(SockStream &s, Package &pkg)
{
	AClient& c = getClientBySockStream(s);
	std::cout << "[IRC]<" << c.getStream().getSocket() << ">" << pkg.getRawData();
	Package pack = Package(this->_protocol, std::string("<") + std::to_string(c.getStream().getSocket()) + "> " + pkg.getRawData());
	this->execute(c, pkg.getData());
	this->sendAll(pack, &s);
}

void							IRCServer::_onClientQuit(SockStream &s)
{
	std::cout << "[IRC] Client " << s.getSocket() << " disconnected." << std::endl;

	Package pack = Package(this->_protocol, std::string("<") + std::to_string(s.getSocket()) + "> disconnected.\r\n");
	//TODO remove client from lists
	this->sendAll(pack, &s);
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
	if (sep < data.size())
	{
		command = data.substr(0, sep);
		args = data.substr(sep);
	}
	switch (client.getId())
	{
		case Client::value_type:
			if (this->_userCommands.count(command) == 1)
				(this->*(this->_userCommands[command]))(dynamic_cast<Client&>(client), args);
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

bool		IRCServer::userCommandUser(Client & client, std::string cmd)
{
	std::cout << "<" << client.getStream().getSocket() << "> Command <USER> with args: " << cmd << std::endl;
	return true;
}

bool		IRCServer::userCommandPass(Client & client, std::string cmd)
{
	std::cout << "<" << client.getStream().getSocket() << "> Command <PASS> with args: " << cmd << std::endl;
	client.setPassword(cmd);
	return true;
}

bool		IRCServer::userCommandNick(Client & client, std::string cmd)
{
	std::cout << "<" << client.getStream().getSocket() << "> Command <NICK> with args: " << cmd << std::endl;
	if (client.getPassword().empty())
		return false;
	return true;
}

/* ************************************************************************** */