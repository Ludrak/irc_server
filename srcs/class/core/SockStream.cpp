#include "SockStream.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

//TODO CHeck return values
SockStream::SockStream()
{
	std::cout << "default SockStream constructor" << std::endl;
	this->_createSocket("127.0.0.1", 8080);
}

SockStream::SockStream(const std::string &host, uint16_t port)
{
	this->_createSocket(host, port);
}

SockStream::SockStream(int socket, const sockaddr_in &address) : _socket(socket), _addr(address)
{
}


SockStream::SockStream( const SockStream & src )
{
	(void) src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

SockStream::~SockStream()
{
	close(this->_socket);
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

SockStream &				SockStream::operator=( SockStream const & rhs )
{
	//if ( this != &rhs )
	//{
		(void) rhs;
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, SockStream const & i )
{
	//o << "Value = " << i.getValue();
	(void) i;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void				SockStream::_createSocket(const std::string &host, uint16_t port, sa_family_t family, int sock_type)
{
	this->_socket = socket(family, sock_type, 0);
	if (this->_socket < 0)
		throw SockStream::SocketCreationException();
	fcntl(this->_socket, F_SETFL, O_NONBLOCK);
	
	bzero(reinterpret_cast<void *>(&this->_addr), sizeof(this->_addr));
	this->_addr.sin_family = family;
	this->_addr.sin_addr.s_addr = inet_addr(host.c_str());
	this->_addr.sin_port = htons(port);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */