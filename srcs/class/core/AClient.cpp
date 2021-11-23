#include "AClient.hpp"

AClient::AClient(void) : ASockManager()
{
	Logger::debug("Constructor AClient");
}

AClient::~AClient(void)
{
}


bool			AClient::connectOn(const std::string host, const ushort port, IProtocol &protocol)
{
	SockStream  *s = new SockStream(host, port, protocol);
	if (connect(s->getSocket(), reinterpret_cast<const sockaddr*>(&s->getAddress()), sizeof(s->getAddress())) != 0)
		throw AClient::ConnectionException();
	s->setType(CLIENT);
	this->addSocket(*s);
#ifdef KQUEUE
	struct kevent new_event;
	EV_SET(&new_event, s->getSocket(), EVFILT_READ, EV_ADD, NOTE_LOWAT, 1, NULL);
	this->_k_events.push_back(new_event);
#endif
	this->_onConnect(*s);
	return (true);
}




t_pollevent		AClient::_pollInClients(SockStream & sock)
{
	char	buffer[RECV_BUFFER_SZ] = { 0 };
	size_t	byte_size = recv(sock.getSocket(), buffer, RECV_BUFFER_SZ, MSG_DONTWAIT);
	if (byte_size < 0)
		return (POLL_ERR);
	else if (byte_size == 0)
	{
		this->_onQuit(sock);
#ifdef KQUEUE
		for (std::vector<struct kevent>::iterator it = this->_k_events.begin(); it != this->_k_events.end(); ++it)
		{
			if (it->ident == sock.getSocket())
			{
				this->_k_events.erase(it);
				//kevent(this->_kq, this->_k_events.data(), this->_k_events.size(), NULL, 0, (struct timespec *)(0));
				break;
			}
		}
#endif
		this->delSocket(sock);
		return (POLL_DELETE);
	}
	sock.getReceivedData().addData(buffer);			
	while (!sock.getReceivedData().isInvalid()){
		this->_onRecv(sock, sock.getReceivedData());
		sock.getReceivedData().flush();
	}
	return (POLL_SUCCESS);
}



t_pollevent		AClient::_pollOutClients(SockStream & sock)
{
	Package	*current_pkg = *sock.getPendingData().begin();

	if (!current_pkg)
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
		return (POLL_ERR);
	}
	char 	buffer[SEND_BUFFER_SZ] = { 0 };
	size_t	buffer_sz = current_pkg->getRawData().size() > SEND_BUFFER_SZ ? SEND_BUFFER_SZ : current_pkg->getRawData().size();
	std::memcpy(buffer, current_pkg->getRawData().c_str(), buffer_sz);

	size_t byte_size = send(current_pkg->getRecipient()->getSocket(), buffer, buffer_sz, 0);
	current_pkg->nflush(byte_size);

	if (current_pkg->isInvalid() || current_pkg->getRawData().empty())
	{
		delete current_pkg;
		sock.getPendingData().remove(current_pkg);
		if (sock.getPendingData().size() == 0)
		{
#ifdef 	POLL
			sock.delPollEvent(POLLOUT);
#elif	defined(EPOLL)
#elif	defined(SELECT)
			sock.deselectIO(SELECT_IO_WRITE);
#elif	defined(KQUEUE)
		for (std::vector<struct kevent>::iterator it = this->_k_events.begin();
			it != this->_k_events.end();
			++it)
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



t_pollevent		AClient::_pollFromServers(SockStream & sock, int event)
{	
	/* client in sock has returned positive event */
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
		t_pollevent ev = this->_pollInClients(sock);
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
		this->_pollOutClients(sock);
	}
	return (POLL_SUCCESS);
}


t_pollevent     AClient::_onPollEvent(int socket, int event)
{
	SockStream  *sock = this->_sockets[socket];
	if (!sock)
		return (POLL_NOTFOUND);
	if (sock->getType() == CLIENT)
		return (this->_pollFromServers(*sock, event));
	return (POLL_NOTFOUND);
}

void			AClient::sendServerPackage( Package *pkg, SockStream &server_recipient )
{
#ifdef	POLL
	server_recipient.setPollEvent(POLLOUT);
#elif 	defined(EPOLL)
#elif	defined(SELECT)
	server_recipient.selectIO(SELECT_IO_WRITE);
#elif	defined(KQUEUE)
	for (std::vector<struct kevent>::iterator it = this->_k_events.begin(); it != this->_k_events.end(); ++it)
		if (it->ident == server_recipient.getSocket())
		{
			server_recipient.delkQueueEvents(this->_kq, this->_k_events, *it, EVFILT_READ);
			server_recipient.setkQueueEvents(this->_kq, this->_k_events, *it, EVFILT_WRITE);
		}
#endif
	pkg->setRecipient(&server_recipient);
	server_recipient.getPendingData().push_back(pkg);
}
