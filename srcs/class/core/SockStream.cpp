#include "SockStream.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

//TODO CHeck return values
SockStream::SockStream(IProtocol & protocol) : _protocol(&protocol), _recieved_data(protocol), _pending_data(protocol)
{
	std::cout << "default SockStream constructor" << std::endl;
	this->_createSocket("127.0.0.1", 8080);
}

SockStream::SockStream(const std::string &host, uint16_t port, IProtocol & protocol) : _protocol(&protocol), _recieved_data(protocol), _pending_data(protocol)
{
	this->_createSocket(host, port);
}

SockStream::SockStream(int socket, const sockaddr_in &address, IProtocol & protocol) : _socket(socket), _addr(address), _protocol(&protocol), _recieved_data(protocol), _pending_data(protocol)
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

size_t							SockStream::recieve(size_t read_bufsz)
{
	std::vector<char> 	buf(read_bufsz);
	size_t 				byte_size;
	
	byte_size = recv(this->_socket, reinterpret_cast<void *>(buf.data()), read_bufsz, MSG_DONTWAIT);
	if (byte_size <= 0) 
		return (byte_size);
	this->_recieved_data.addData(std::string (buf.begin(), buf.begin() + byte_size));
	return (byte_size);
}

size_t						send(size_t send_bufsz)
{
	(void) send_bufsz;
	return (0);
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

IProtocol					*SockStream::getProtocol( void ) const
{
	return this->_protocol;
}


void							SockStream::setPackageProtocol(IProtocol &proto)
{
	this->_protocol = &proto;
}

Package							&SockStream::getRecievedData()
{
	return (this->_recieved_data);
}

Package							&SockStream::getPendingData()
{
	return (this->_pending_data);
}


/* ************************************************************************** */