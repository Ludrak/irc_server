#include "AServer.hpp"

/*
** -------------------------------- STATICS --------------------------------
*/

uint		AServer::_defaultMaxConnections = 50;


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AServer::AServer( const std::string &host ) : ASockManager(), _host(host), _maxConnections(AServer::_defaultMaxConnections)
{

}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AServer::~AServer()
{
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/* Server */
void						AServer::run()
{
	for (std::map<int, SockStream *>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++)
	{
		/* TODO maybe set a by port default max connection as std::map<ushort port, uint max_connections> */
		if (listen(it->first, this->_defaultMaxConnections) != 0)
		{
			std::cerr << "FATAL: can't listen on " << inet_ntoa(it->second->getSockaddr().sin_addr) << ":" << ntohs(it->second->getSockaddr().sin_port) << std::endl;
			return ;
		}
	}
	ASockManager::run();
}



t_pollevent					AServer::_pollFromServer(int socket, int event)
{
	SockStream	*sock = this->_sockets[socket];

	if (sock == NULL || sock->getType() != SERVER || ~event & POLLIN)
		return (POLL_NOTFOUND);
	
	/* server socket was written, accept incomming connection */
	sockaddr_in		client_addr;
	socklen_t		client_addr_len;
	int				client_sock;
	
	client_sock = accept(socket, reinterpret_cast<sockaddr *>(&client_addr), &client_addr_len);
	if (client_sock <= 0)
	{
		std::cerr << "FATAL: accept() returned a negative value: " << strerror(errno) << std::endl;
		return (POLL_NOACCEPT);
	}
	SockStream	*client_ss = new SockStream(client_sock, client_addr, *sock->getProtocol());
	this->_sockets.insert(std::make_pair(client_sock, client_ss));
	return (POLL_SUCCESS);
}



t_pollevent					AServer::_pollFromClients(int socket, int event)
{
	SockStream	*sock = this->_sockets[socket];

	if (sock == NULL || sock->getType() != CLIENT)
		return (POLL_NOTFOUND);
	
	/* client in sock has returned positive event */
	if (event & POLLIN)
	{
		/* client socket is readable */
		t_pollevent ev = this->_pollInClients(sock);
		if (ev == POLL_DELETE)
			return (POLL_DELETE);
	}
	if (event & POLLOUT)
	{
		/* client socket is writable */
		this->_pollOutClients(sock);
	}
	return (POLL_SUCCESS);
}



t_pollevent					AServer::_pollInClients(SockStream *const sock)
{
	char	buffer[RECV_BUFFER_SZ] = { 0 };
	size_t	byte_size = recv(sock->getSocket(), buffer, RECV_BUFFER_SZ, MSG_DONTWAIT);
	if (byte_size < 0)
		return (POLL_ERR);
	else if (byte_size == 0)
	{
		this->_onClientQuit(*sock);
		return (POLL_DELETE);
	}
	sock->getRecievedData().addData(buffer);			
	while (!sock->getRecievedData().isInvalid()){
		this->_onClientRecv(*sock, sock->getRecievedData());
		if (!sock)
			return (POLL_DELETE);
		sock->getRecievedData().flush();
	}
	return (POLL_SUCCESS);
}



t_pollevent					AServer::_pollOutClients(SockStream *const sock)
{
	Package	&current_pkg = **sock->getPendingData().begin();

	char 	buffer[SEND_BUFFER_SZ] = { 0 };
	size_t	buffer_sz = current_pkg.getRawData().size() > SEND_BUFFER_SZ ? SEND_BUFFER_SZ : current_pkg.getRawData().size();
	std::memcpy(buffer, current_pkg.getRawData().c_str(), buffer_sz);

	size_t byte_size = send(current_pkg.getRecipient()->getSocket(), buffer, buffer_sz, 0);
	current_pkg.nflush(byte_size);

	if (current_pkg.isInvalid() || current_pkg.getRawData().empty())
	{
		delete &current_pkg;
		sock->getPendingData().remove(&current_pkg);
		if (sock->getPendingData().size() == 0)
			sock->delPollEvent(POLLOUT);
	}
	return (POLL_SUCCESS);
}




t_pollevent					AServer::_onPollEvent(int socket, int event)
{
	/* trying to read events from clients first */
	t_pollevent ev = this->_pollFromClients(socket, event);
	if (ev != POLL_NOTFOUND)
		return ev;
	/* if no client has the corresponding socket, search on server sockets */
	ev = this->_pollFromServer(socket, event);
	if (ev != POLL_NOTFOUND)
		return ev;
	std::cerr << "FATAL: incomming data from unknown socket" << std::endl;
	return (POLL_NOTFOUND);
}



bool						AServer::listenOn( ushort port, IProtocol &protocol )
{
	SockStream *new_sock = new SockStream(this->_host, port, protocol);
	if (bind(new_sock->getSocket(), reinterpret_cast<const sockaddr *>(&new_sock->getSockaddr()), sizeof(new_sock->getSockaddr())) != 0)
		throw AServer::AddressBindException();
	this->_sockets.insert(std::make_pair(new_sock->getSocket(), new_sock));
	return true;
}




/* Clients */

void						AServer::sendPackage( Package *pkg, SockStream &recipient)
{
	recipient.setPollEvent(POLLOUT);
	pkg->setRecipient(&recipient);
	recipient.getPendingData().push_back(pkg);
}



void						AServer::sendAll( const Package &package, const SockStream *except )
{
	for (std::map<int, SockStream *>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++)
	{
		if (!except || it->second != except)
			this->sendPackage(new Package(package), *it->second);
	}
}



void						AServer::kick( SockStream &client )
{
	if (this->_sockets[client.getSocket()] != NULL)
	{
		this->_onClientQuit(client);
		int sock = client.getSocket();
		delete &client;
		this->_sockets.erase(sock);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

uint						AServer::getMaxConnection( void ) const
{
	return this->_maxConnections;
}

void						AServer::setMaxConnection( uint nb)
{
	this->_maxConnections = nb;
}

/* ************************************************************************** */