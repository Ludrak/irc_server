#include "ASockStream.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

//TODO CHeck return values
ASockStream::ASockStream()
{
	std::cout << "default ASockStream constructor" << std::endl;
	this->_createSocket("127.0.0.1", 8080);
}

ASockStream::ASockStream(const std::string &host, uint16_t port)
{
	this->_createSocket(host, port);
}

ASockStream::ASockStream(int socket, const sockaddr_in &address) : _socket(socket), _addr(address)
{
}


ASockStream::ASockStream( const ASockStream & src )
{
	(void) src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ASockStream::~ASockStream()
{
	close(this->_socket);
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ASockStream &				ASockStream::operator=( ASockStream const & rhs )
{
	//if ( this != &rhs )
	//{
		(void) rhs;
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, ASockStream const & i )
{
	//o << "Value = " << i.getValue();
	(void) i;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void				ASockStream::_createSocket(const std::string &host, uint16_t port, sa_family_t family, int sock_type)
{
	this->_socket = socket(family, sock_type, 0);
	if (this->_socket < 0)
		throw ASockStream::SocketCreationException();
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