#include "IRCServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
//REVIEW Server name maximum 63 character
//REVIEW nickname name maximum 9 character

IRCServer::IRCServer(int port, const std::string & password, const std::string &host) : AServer(host, port, this->_protocol), _protocol(IRCProtocol()), _password(password), _network_password("")
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
}

void							IRCServer::_onClientRecv(SockStream &s, const Package pkg)
{
	std::cout << "[IRC]<" << s.getSocket() << "> " << pkg.getData();
}

void							IRCServer::_onClientQuit(SockStream &s)
{
	std::cout << "[IRC] Client " << s.getSocket() << " disconnected." << std::endl;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const SockStream&					IRCServer::getForwardSocket( void ) const
{
	return this->_forward_socket;
}

/* ************************************************************************** */