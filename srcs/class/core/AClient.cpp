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
		this->delSocket(sock);
		return (POLL_DELETE);
	}
	sock.getRecievedData().addData(buffer);			
	while (!sock.getRecievedData().isInvalid()){
		this->_onRecv(sock, sock.getRecievedData());
		sock.getRecievedData().flush();
	}
	return (POLL_SUCCESS);
}



t_pollevent		AClient::_pollOutClients(SockStream & sock)
{
	Package	&current_pkg = **sock.getPendingData().begin();

	char 	buffer[SEND_BUFFER_SZ] = { 0 };
	size_t	buffer_sz = current_pkg.getRawData().size() > SEND_BUFFER_SZ ? SEND_BUFFER_SZ : current_pkg.getRawData().size();
	std::memcpy(buffer, current_pkg.getRawData().c_str(), buffer_sz);

	size_t byte_size = send(current_pkg.getRecipient()->getSocket(), buffer, buffer_sz, 0);
	current_pkg.nflush(byte_size);

	if (current_pkg.isInvalid() || current_pkg.getRawData().empty())
	{
		delete &current_pkg;
		sock.getPendingData().remove(&current_pkg);
		if (sock.getPendingData().size() == 0)
		{
#ifndef KQUEUE
			sock.delPollEvent(POLLOUT);
#else
		for (std::vector<struct kevent>::iterator it = this->_k_events.begin();
			it != this->_k_events.end();
			++it)
			if (it->ident == sock.getSocket())
				sock.delkQueueEvents(*it, EVFILT_WRITE);
#endif
		}
	}
	return (POLL_SUCCESS);
}



t_pollevent		AClient::_pollFromServers(SockStream & sock, int event)
{	
	/* client in sock has returned positive event */
#ifndef KQUEUE
	if (event & POLLIN)
#else
	if (event == EVFILT_READ)
#endif
	{
		/* client socket is readable */
		t_pollevent ev = this->_pollInClients(sock);
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
#ifndef KQUEUE
	server_recipient.setPollEvent(POLLOUT);
#else
	for (std::vector<struct kevent>::iterator it = this->_k_events.begin(); it != this->_k_events.end(); ++it)
		if (it->ident == server_recipient.getSocket())
			server_recipient.setkQueueEvents(*it, EVFILT_WRITE);
#endif
	pkg->setRecipient(&server_recipient);
	server_recipient.getPendingData().push_back(pkg);
}
