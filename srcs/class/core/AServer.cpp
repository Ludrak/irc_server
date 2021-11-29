#include "AServer.hpp"

uint					AServer::_defaultMaxConnections = 30;


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AServer::AServer( const std::string &host, const std::string &ssl_cert_path, const std::string &ssl_key_path ) 
: ASockManager(ssl_cert_path, ssl_key_path), _host(host), _running(false), _maxConnections(AServer::_defaultMaxConnections)
{
	Logger::debug("Constructor AServer: " + this->_host);
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
	this->_running = true;
	for (std::map<ushort, SockStream *>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++)
	{
		/* TODO maybe set a by port default max connection as std::map<ushort port, uint max_connections> */
		if (it->second->getType() == SERVER && listen(it->first, this->_defaultMaxConnections) != 0)
		{
			Logger::error(ntos("can't listen on ") + it->second->getIP() + " : " + strerror(errno));
			return ;
		}
	}
	ASockManager::run();
}



t_pollevent					AServer::_pollFromServer(int socket, int event)
{
	std::map<ushort, SockStream*>::iterator it = this->_sockets.find(socket);
#ifdef	POLL
	if (it == this->_sockets.end() || it->second->getType() != SERVER || ~event & POLLIN)
#elif	defined(EPOLL)
#elif	defined(SELECT)
	if (it == this->_sockets.end() || it->second->getType() != SERVER || ~event & SELECT_IO_READ)
#elif	defined(KQUEUE)
	if (it == this->_sockets.end() || it->second->getType() != SERVER || ~event == EVFILT_READ)
#endif
		return (POLL_NOTFOUND);
	/* server socket was written, accept incomming connection */
	sockaddr_in		client_addr;
	socklen_t		client_addr_len;
	int				client_sock;

	client_sock = accept(socket, reinterpret_cast<sockaddr *>(&client_addr), &client_addr_len);
	if (client_sock <= 0)
	{
		Logger::error(ntos("accept() returned a negative value: ") + strerror(errno));
		return (POLL_NOACCEPT);
	}
	
	SockStream	*client_ss = new SockStream(client_sock, client_addr, *it->second->getProtocol(), this->_sockets[socket]->hasTLS(), this->_ssl_ctx);
	if (client_ss->hasTLS())
		client_ss->acceptSSL();
	client_ss->setType(REMOTE_CLIENT);
	this->addSocket(*client_ss);
	this->_onClientJoin(*client_ss);
#ifdef KQUEUE
	struct kevent new_event;
	EV_SET(&new_event, client_sock, EVFILT_READ, EV_ADD, NOTE_LOWAT, 1, NULL);
	this->_k_events.push_back(new_event);
#endif
	return (POLL_SUCCESS);
}



t_pollevent					AServer::_pollFromClients(int socket, int event)
{
	std::map<ushort, SockStream*>::iterator it = this->_sockets.find(socket);
	if (it == this->_sockets.end() || it->second->getType() != REMOTE_CLIENT)
		return (POLL_NOTFOUND);
	/* client socket has returned positive event */
#ifdef	POLL
	if (event & POLLIN)
#elif	defined(EPOLL)
#elif	defined(SELECT)
	if (event & SELECT_IO_READ)
#elif	defined(KQUEUE)
	if (event == EVFILT_READ)
#endif
	{
		/* client socket is readable */
		t_pollevent ev = this->_pollInClients(*it->second);
		if (ev == POLL_DELETE)
			return (POLL_DELETE);
	}
#ifdef	POLL
	if (event & POLLOUT)
#elif	defined(EPOLL)
#elif	defined(SELECT)
	if (event & SELECT_IO_WRITE)
#elif	defined(KQUEUE)
	if (event == EVFILT_WRITE)
#endif
	{
		/* client socket is writable */
		this->_pollOutClients(*it->second);
	}
	return (POLL_SUCCESS);
}



t_pollevent					AServer::_pollInClients(SockStream & sock)
{
	char	buffer[RECV_BUFFER_SZ + 1] = { 0 };
	ssize_t	byte_size;
	if (sock.hasTLS() && sock.getSSL())
	{
		byte_size = SSL_read(sock.getSSL(), buffer, RECV_BUFFER_SZ);
	}
	else
		byte_size = recv(sock.getSocket(), buffer, RECV_BUFFER_SZ, MSG_DONTWAIT);
	if (byte_size < 0)
	{
		if (sock.hasTLS() && sock.getSSL())
			ERR_print_errors_fp(stderr);

		Logger::error(std::string("AServer: recv() failed : ") + strerror(errno) + std::string(" on socket <" + ntos(sock.getSocket()) + ">."));
		return POLL_ERR;
	}
	else if (byte_size == 0) 
	{
		this->disconnect(sock);
		return (POLL_DELETE);
	}
	buffer[byte_size] = '\0';
	sock.getReceivedData().addData(buffer);			
	while (!sock.getReceivedData().isInvalid()){
		int socket = sock.getSocket();
		Logger::debug("received package from " + sock.getIP());
		this->_onClientRecv(sock, sock.getReceivedData());
		if (this->_sockets.count(socket) == 0)
			return (POLL_DELETE);
		sock.getReceivedData().flush();
	}
	return (POLL_SUCCESS);
}



t_pollevent					AServer::_pollOutClients(SockStream & sock)
{
	Package	*current_pkg = *sock.getPendingData().begin();

	if (!current_pkg) {
#ifdef	POLL
			sock.delPollEvent(POLLOUT);
#elif	defined(EPOLL)
#elif	defined(SELECT)
			sock.deselectIO(SELECT_IO_WRITE);
#elif	defined(KQUEUE)
			for (std::vector<struct kevent>::iterator it = this->_k_events.begin(); it != this->_k_events.end(); ++it)
				if (it->ident == sock.getSocket())
				{
					sock.delkQueueEvents(this->_kq, this->_k_events, *it, EVFILT_WRITE);
					sock.setkQueueEvents(this->_kq, this->_k_events, *it, EVFILT_READ);
				}
#endif
		return (POLL_ERR);
	}
	char 	buffer[SEND_BUFFER_SZ] = { 0 };
	size_t	buffer_sz = current_pkg->getRawData().size() > SEND_BUFFER_SZ ? SEND_BUFFER_SZ : current_pkg->getRawData().size();
	std::memcpy(buffer, current_pkg->getRawData().c_str(), buffer_sz);

	ssize_t	byte_size;
	if (sock.hasTLS() && sock.getSSL())
		byte_size = SSL_write(current_pkg->getRecipient()->getSSL(), buffer, buffer_sz);
	else
		byte_size = send(current_pkg->getRecipient()->getSocket(), buffer, buffer_sz, 0);
	if (byte_size < 0)
	{
		Logger::error("AServer: send() error on " + sock.getIP() + ntos(" : ") + strerror(errno));
		return (POLL_ERR);
	}
	current_pkg->nflush(byte_size);

	if (current_pkg->isInvalid() || current_pkg->getRawData().empty())
	{
		Logger::debug("sent & deleted package " + ntos(current_pkg) + ntos(" of ") + sock.getIP() + " explosive: " + ntos(current_pkg->isExplosive()));
		if (current_pkg->isExplosive())
		{		
			Logger::debug("activated explosive package " + ntos(current_pkg) + ntos(" of ") + sock.getIP());
			delete current_pkg;
			this->disconnect(sock);
			return (POLL_DELETE);
		}
		delete current_pkg;
		sock.getPendingData().remove(current_pkg);
		if (sock.getPendingData().size() == 0)
		{
#ifdef	POLL
			sock.delPollEvent(POLLOUT);
#elif	defined(EPOLL)
#elif	defined(SELECT)
			sock.deselectIO(SELECT_IO_WRITE);
#elif	defined(KQUEUE)
			for (std::vector<struct kevent>::iterator it = this->_k_events.begin(); it != this->_k_events.end(); ++it)
				if (it->ident == sock.getSocket())
				{
					sock.delkQueueEvents(this->_kq, this->_k_events, *it, EVFILT_WRITE);
					sock.setkQueueEvents(this->_kq, this->_k_events, *it, EVFILT_READ);
				}
#endif
		}
	}
	return (POLL_SUCCESS);
}




t_pollevent					AServer::_onPollEvent(int socket, int event)
{
#ifdef KQUEUE
	if (this->_sockets[socket] && event == EV_EOF)
		return POLL_DELETE;
#endif

	/* trying to read events from clients first */
	t_pollevent ev = this->_pollFromClients(socket, event);
	if (ev != POLL_NOTFOUND)
		return ev;

	/* if no client has the corresponding socket, search on server sockets */
	ev = this->_pollFromServer(socket, event);
	if (ev != POLL_NOTFOUND)
		return ev;
	
	/* data wasn't addressed to server */
	return (POLL_NOTFOUND);
}



bool						AServer::listenOn( ushort port, IProtocol &protocol , const bool useTLS)
{
	SockStream *new_sock = new SockStream(this->_host, port, protocol, useTLS);
	new_sock->setType(SERVER);
	if (bind(new_sock->getSocket(), reinterpret_cast<const sockaddr *>(&new_sock->getAddress()), sizeof(new_sock->getAddress())) != 0)
	{
		Logger::error(ntos("Can't bind on ") + new_sock->getIP() + ": " + ntos(strerror(errno)));
		return (false);
	}
	this->_sockets.insert(std::make_pair(new_sock->getSocket(), new_sock));
	if (this->_running && listen(new_sock->getSocket(), this->_maxConnections) != 0)
	{
		Logger::error(ntos("Can't listen on ")  + new_sock->getIP() + ": " + ntos(strerror(errno)));
		return (false);
	}
	if (useTLS)
	{
		new_sock->initTLS(this->_ssl_ctx);
		SSL_set_connect_state (new_sock->getSSL());
		//SSL_set_accept_state (new_sock->getSSL());
		SSL_set_fd(new_sock->getSSL(), new_sock->getSocket());
	}
	return (true);
}


void		    			AServer::disconnect( SockStream &client )
{
	Logger::debug("disconnecting socket: " + ntos(client.getSocket()));

#ifdef KQUEUE
	for (std::vector<struct kevent>::iterator it = this->_k_events.begin(); it != this->_k_events.end(); ++it)
	{
		if (it->ident == client.getSocket())
		{
			this->_k_events.erase(it);
			break;
		}
	}
#endif
	this->_onClientQuit(client);
	this->delSocket(client);
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