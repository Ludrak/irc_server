#include "SockStream.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

//TODO CHeck return values
SockStream::SockStream(IProtocol & protocol) : _poll_events(POLLIN), _protocol(&protocol), _recieved_data(protocol)
{
	Logger::debug("default SockStream constructor");
	this->_createSocket("127.0.0.1", 8080);
}

SockStream::SockStream(const std::string &host, uint16_t port, IProtocol & protocol) : _poll_events(POLLIN), _protocol(&protocol), _recieved_data(protocol)
{
	this->_createSocket(host, port);
}

SockStream::SockStream(int socket, const sockaddr_in &address, IProtocol & protocol) : _socket(socket), _poll_events(POLLIN), _addr(address), _protocol(&protocol), _recieved_data(protocol)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

SockStream::~SockStream()
{
	this->close();
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void							SockStream::_createSocket(const std::string &host, uint16_t port, sa_family_t family, int sock_type)
{
	int option = 1; // TODO add this to a class value
	if ((this->_socket = socket(family, sock_type, 0)) < 0
	|| fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0
	|| setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
		throw SockStream::SocketCreationException();
	bzero(reinterpret_cast<void *>(&this->_addr), sizeof(this->_addr));
	this->_addr.sin_port = htons(port);
	this->_addr.sin_family = family;
	this->_addr.sin_addr.s_addr = inet_addr(host.c_str());
}

void							SockStream::close( void )
{
	::close(this->_socket);
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

IProtocol						*SockStream::getProtocol( void ) const
{
	return this->_protocol;
}

int								SockStream::getPollEvents() const 
{
	return (this->_poll_events);
}
void							SockStream::setPollEvent(int event)
{
	this->_poll_events |= event;
}
void							SockStream::delPollEvent(int event)
{
	this->_poll_events &= ~event;
}


void							SockStream::setPackageProtocol(IProtocol &proto)
{
	this->_protocol = &proto;
}

Package							&SockStream::getRecievedData()
{
	return (this->_recieved_data);
}

std::list<Package*>				&SockStream::getPendingData()
{
	return (this->_pending_data);
}

/* ************************************************************************** */