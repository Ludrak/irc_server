#include "SockStream.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

SockStream::SockStream()
{
	std::cout << "default SockStream constructor" << std::endl;
	this->_createSocket("192.168.1.242", 8080);
}

SockStream::SockStream(const std::string &host, uint16_t port)
{
	this->_createSocket(host, port);
}

SockStream::SockStream(int socket, const sockaddr_in &address) : _socket(socket), _addr(address)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

SockStream::~SockStream()
{
	close(this->_socket);
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void							SockStream::_createSocket(const std::string &host, uint16_t port, sa_family_t family, int sock_type)
{
	if ((this->_socket = socket(family, sock_type, 0)) < 0
	|| fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0)
		throw SockStream::SocketCreationException();
	bzero(reinterpret_cast<void *>(&this->_addr), sizeof(this->_addr));
	this->_addr.sin_port = htons(port);
	this->_addr.sin_family = family;
	this->_addr.sin_addr.s_addr = inet_addr(host.c_str());
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int								SockStream::getSocket( void ) const
{
	return this->_socket;
}

const struct sockaddr_in&		SockStream::getSockaddr( void ) const
{
	return this->_addr;
}

/* ************************************************************************** */