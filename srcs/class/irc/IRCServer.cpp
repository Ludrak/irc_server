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


void							IRCServer::_onClientJoin(SockStream &s)
{
	std::cout << "[IRC] Client " << s.getSocket() << " joined the server !" << std::endl;
	s.setPackageProtocol(this->_protocol);

	Package pack = Package(this->_protocol, std::string("<") + std::to_string(s.getSocket()) + "> joined the server !\r\n");
	for (std::map<int, SockStream *>::iterator it = this->getClients().begin(); it != this->getClients().end(); it++)
	{
		if (it->second != &s)
			s.sendPackage(new Package(pack), *it->second);
	}
}

void							IRCServer::_onClientRecv(SockStream &s, Package pkg)
{
	std::cout << "[IRC]<" << s.getSocket() << "> " << pkg.getData();

	Package pack = Package(this->_protocol, std::string("<") + std::to_string(s.getSocket()) + "> " + pkg.getData());
	for (std::map<int, SockStream *>::iterator it = this->getClients().begin(); it != this->getClients().end(); it++)
	{
		if (it->second != &s)
			s.sendPackage(new Package(pack), *it->second);
	}
}

void							IRCServer::_onClientQuit(SockStream &s)
{
	std::cout << "[IRC] Client " << s.getSocket() << " disconnected." << std::endl;

	Package pack = Package(this->_protocol, std::string("<") + std::to_string(s.getSocket()) + "> disconnected.\r\n");
	for (std::map<int, SockStream *>::iterator it = this->getClients().begin(); it != this->getClients().end(); it++)
	{
		if (it->second != &s) {
			s.sendPackage(new Package(pack), *it->second);
		}
	}
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const SockStream&					IRCServer::getForwardSocket( void ) const
{
	return this->_forward_socket;
}

/* ************************************************************************** */