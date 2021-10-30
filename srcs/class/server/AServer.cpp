#include "AServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AServer::AServer() : ASockStream()
{
	this->_init_server();
}

AServer::AServer( const AServer & src ) : _clients(src._clients)
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AServer::~AServer()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

AServer &				AServer::operator=( AServer const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	(void) rhs;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, AServer const & i )
{
	//o << "Value = " << i.getValue();
	(void ) i;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

bool						AServer::run( void )
{
	if (listen(this->_socket, this->_max_connection) != 0)
		throw AServer::ListenException();
	while (1)
	{
		
	}
	return true;
}

Client						*AServer::_acceptConnection(ASockStream &socket_client)
{
	return new Client(socket_client);
}

bool					AServer::_init_server( void )
{
	if (bind(this->_socket, reinterpret_cast<sockaddr *>(&this->_addr), sizeof(this->_addr)) != 0)
		throw AServer::AddressBindException();
	return true;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

uint					AServer::getMaxConnection( void ) const
{
	return this->_max_connection;
}

void					AServer::setMaxConnection( uint nb)
{
	this->_max_connection = nb;
}

/* ************************************************************************** */