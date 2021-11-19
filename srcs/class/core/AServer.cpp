#include "AServer.hpp"

uint					AServer::_defaultMaxConnections = 30;


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AServer::AServer( const std::string &host ) : ASockManager(), _host(host), _running(false), _maxConnections(AServer::_defaultMaxConnections)
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
#ifndef KQUEUE
	if (it == this->_sockets.end() || it->second->getType() != SERVER || ~event & POLLIN)
#else
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
	SockStream	*client_ss = new SockStream(client_sock, client_addr, *it->second->getProtocol());
	client_ss->setType(REMOTE_CLIENT);
	this->addSocket(*client_ss);
	this->_onClientJoin(*client_ss);
#ifdef KQUEUE
	struct kevent new_event;
	EV_SET(&new_event, client_sock, EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, NULL);
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
#ifndef KQUEUE
	if (event & POLLIN)
#else
	if (event == EVFILT_READ)
#endif
	{
		/* client socket is readable */
		t_pollevent ev = this->_pollInClients(*it->second);
		if (ev == POLL_DELETE)
			return (POLL_DELETE);
	}
#ifndef KQUEUE
	if (event & POLLOUT)
#else
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
	ssize_t	byte_size = recv(sock.getSocket(), buffer, RECV_BUFFER_SZ, MSG_DONTWAIT);
	if (byte_size < 0)
	{
		Logger::error(std::string("recv() failed : ") + strerror(errno) + std::string(" on socket <" + ntos(sock.getSocket()) + ">."));
		return POLL_ERR;
	}
	else if (byte_size == 0) 
	{
		this->disconnect(sock);
		return (POLL_DELETE);
	}
	buffer[byte_size] = '\0';
	sock.getRecievedData().addData(buffer);			
	while (!sock.getRecievedData().isInvalid()){
		int socket = sock.getSocket();
		Logger::debug("recieved package from " + sock.getIP());
		this->_onClientRecv(sock, sock.getRecievedData());
		if (this->_sockets.count(socket) == 0)
			return (POLL_DELETE);
		sock.getRecievedData().flush();
	}
	return (POLL_SUCCESS);
}



t_pollevent					AServer::_pollOutClients(SockStream & sock)
{
	Package	&current_pkg = **sock.getPendingData().begin();

	char 	buffer[SEND_BUFFER_SZ] = { 0 };
	size_t	buffer_sz = current_pkg.getRawData().size() > SEND_BUFFER_SZ ? SEND_BUFFER_SZ : current_pkg.getRawData().size();
	std::memcpy(buffer, current_pkg.getRawData().c_str(), buffer_sz);

	size_t byte_size = send(current_pkg.getRecipient()->getSocket(), buffer, buffer_sz, 0);
	if (byte_size < 0)
	{
		Logger::error("send() error on " + sock.getIP() + ntos(" : ") + strerror(errno));
		return (POLL_ERR);
	}
	current_pkg.nflush(byte_size);

	if (current_pkg.isInvalid() || current_pkg.getRawData().empty())
	{
		Logger::debug("sent & deleted package " + ntos(&current_pkg) + ntos(" of ") + sock.getIP() + " explosive: " + ntos(current_pkg.isExplosive()));
		if (current_pkg.isExplosive())
		{		
			Logger::debug("activated explosive package " + ntos(&current_pkg) + ntos(" of ") + sock.getIP());
			delete &current_pkg;
			this->disconnect(sock);
			return (POLL_DELETE);
		}
		delete &current_pkg;
		sock.getPendingData().remove(&current_pkg);
		if (sock.getPendingData().size() == 0)
		{
#ifndef KQUEUE
			sock.delPollEvent(POLLOUT);
#else	
			for (std::vector<struct kevent>::iterator it = this->_k_events.begin(); it != this->_k_events.end(); ++it)
				if (it->ident == sock.getSocket())
					sock.delkQueueEvents(*it, EVFILT_WRITE);
#endif
		}
	}
	return (POLL_SUCCESS);
}




t_pollevent					AServer::_onPollEvent(int socket, int event)
{
	if (this->_sockets[socket] && event == EV_EOF) {
		return POLL_DELETE;
	}

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



bool						AServer::listenOn( ushort port, IProtocol &protocol )
{
	SockStream *new_sock = new SockStream(this->_host, port, protocol);
	new_sock->setType(SERVER);
	if (bind(new_sock->getSocket(), reinterpret_cast<const sockaddr *>(&new_sock->getAddress()), sizeof(new_sock->getAddress())) != 0)
	{
		Logger::error(ntos("Can't bind on ") + new_sock->getIP() + " : " + ntos(strerror(errno)));
		return (false);
	}	
	this->_sockets.insert(std::make_pair(new_sock->getSocket(), new_sock));
	if (this->_running && listen(new_sock->getSocket(), this->_maxConnections) != 0)
	{
		Logger::error(ntos("Can't listen on ")  + new_sock->getIP() + " : " + ntos(strerror(errno)));
		return (false);
	}
	return (true);
}


void		    			AServer::disconnect( SockStream &client )
{
	Logger::info("disconnect client:" + client.getIP());
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