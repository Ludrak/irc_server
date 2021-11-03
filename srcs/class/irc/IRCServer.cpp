#include "IRCServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
//REVIEW Server name maximum 63 character
//REVIEW nickname name maximum 9 character

IRCServer::IRCServer(int port, const std::string & password, const std::string &host) : AServer(this->_protocol, host, port), _protocol(IRCProtocol()), _forward_socket(this->_protocol), _password(password), _network_password("")
{
	std::cout << "IRCServer constructor" << "\n";
	std::cout << "IRCServer host:" << host << "\n";
	std::cout << "IRCServer port:" << port << "\n";
	std::cout << "IRCServer password:" << password << "\n" << std::endl;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

IRCServer::~IRCServer()
{
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


SockStream&						IRCServer::_onClientJoin(SockStream &s)
{
	SockStream *new_client = new Client(s);
	int idx = s.getSocket();
	delete this->_clients.at(idx);
	this->_clients.at(idx) = new_client;
	(*new_client).setPackageProtocol(this->_protocol);
	std::cout << "[IRC] Client " << idx << " joined the server !" << std::endl;
	Package pack = Package(this->_protocol, std::string("<") + std::to_string(idx) + "> joined the server !\r\n");
	this->sendAll(pack, &s);
	return (*new_client);
}

void							IRCServer::_onClientRecv(SockStream &s, Package &pkg)
{
	Client *c = reinterpret_cast<Client *>(&s);

	std::cout << "[IRC]<" << c->getSocket() << ">" << pkg.getRawData();
	Package pack = Package(this->_protocol, std::string("<") + std::to_string(c->getSocket()) + "> " + pkg.getRawData());
	c->execute(pkg.getData());
	this->sendAll(pack, c);
}

void							IRCServer::_onClientQuit(SockStream &s)
{
	std::cout << "[IRC] Client " << s.getSocket() << " disconnected." << std::endl;

	Package pack = Package(this->_protocol, std::string("<") + std::to_string(s.getSocket()) + "> disconnected.\r\n");
	this->sendAll(pack, &s);
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const SockStream&					IRCServer::getForwardSocket( void ) const
{
	return this->_forward_socket;
}

/* ************************************************************************** */