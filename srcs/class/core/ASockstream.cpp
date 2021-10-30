#include "ASockStream.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

//TODO CHeck return values
ASockStream::ASockStream() : _socket_familly(AF_INET), _socket_type(SOCK_STREAM)
{
	this->_socket = socket(_socket_familly, _socket_type, 0);
	fcntl(this->_socket, F_SETFL, O_NONBLOCK);
	bzero(reinterpret_cast<void *>(&this->_serv_addr), sizeof(this->_serv_addr));
	this->_serv_addr.sin_family = this->_socket_familly;
	this->_serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

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


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */