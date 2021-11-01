#include "SockStream.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

//TODO CHeck return values
SockStream::SockStream(IProtocol & protocol) : _poll_events(POLLIN), _protocol(&protocol), _server(NULL), _recieved_data(protocol)
{
	std::cout << "default SockStream constructor" << std::endl;
	this->_createSocket("127.0.0.1", 8080);
}

SockStream::SockStream(const std::string &host, uint16_t port, IProtocol & protocol) : _poll_events(POLLIN), _protocol(&protocol), _server(NULL), _recieved_data(protocol)
{
	this->_createSocket(host, port);
}

SockStream::SockStream(const AServer *server, int socket, const sockaddr_in &address, IProtocol & protocol) : _socket(socket), _poll_events(POLLIN), _addr(address), _protocol(&protocol),  _server(server), _recieved_data(protocol)
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

void							SockStream::sendPackage( Package *pkg, const SockStream &recipient)
{
	this->setPollEvent(POLLOUT);
	pkg->setRecipient(&recipient);
	this->_pending_data.push_back(pkg);
	//this->_pending_data.addData(pkg.getData());
	//this->_pending_data.setRecipient(&recipient);
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

AServer							*SockStream::getServer() const
{
	return (const_cast<AServer *>(this->_server));
}


/* ************************************************************************** */