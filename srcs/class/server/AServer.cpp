#include "AServer.hpp"

/*
** -------------------------------- STATICS --------------------------------
*/

uint		AServer::default_max_connections = 50;


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AServer::AServer() : SockStream(), _max_connection(AServer::default_max_connections)
{
	this->_init_server();
}

AServer::AServer( const AServer & src ) : SockStream(src), _clients(src._clients), _max_connection(AServer::default_max_connections)
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
	std::vector<struct pollfd> 	poll_fds;
	struct pollfd pfd = {.fd = this->_socket, .events = POLLIN, .revents = 0};
	poll_fds.push_back(pfd);
	std::cout << "poll_fds.size = " << poll_fds.size() << std::endl;
	while (1)
	{
		std::cout << "poll_fds.size = " << poll_fds.size() << std::endl;
		if (poll(reinterpret_cast<pollfd*>(&(*poll_fds.begin())), poll_fds.size(), -1) == -1)
			throw AServer::PollException();
		if (poll_fds.at(0).revents | POLLIN)
			this->_clients.push_back(this->_acceptConnection());
	}
	return true;
}

Client						AServer::_acceptConnection()
{	
	std::cout << "Incoming connection" << std::endl;
	sockaddr_in         cli_addr;
	int					socket_client;
	socklen_t			cli_len = sizeof(cli_addr);

	socket_client = accept(this->_socket, reinterpret_cast<struct sockaddr *>(&cli_addr), &cli_len);
	if (socket_client < 0)
		throw AServer::IncomingConnectionException();

	std::cout << "connection accepted" << std::endl;
	return		Client(socket_client, cli_addr);
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