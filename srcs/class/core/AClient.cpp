
#include "AClient.hpp"

AClient::AClient(void) : ASockManager()
{
}

AClient::~AClient(void)
{
}


bool			AClient::connectOn(const std::string host, const ushort port, IProtocol &protocol)
{
	SockStream  *s = new SockStream(host, port, protocol);
	if (connect(s->getSocket(), reinterpret_cast<const sockaddr*>(&s->getSockaddr()), sizeof(s->getSockaddr())) != 0)
	{
		std::cerr << "can't connect on : " << inet_ntoa(s->getSockaddr().sin_addr) << ":" << ntohs(s->getSockaddr().sin_port) << ": " << strerror(errno) << std::endl;
		return (false);
	}
	s->setType(CLIENT);
	this->addSocket(s);
	this->_onConnect(*s);
	return (true);
}




t_pollevent		AClient::_pollInClients(SockStream *const sock)
{
	char	buffer[RECV_BUFFER_SZ] = { 0 };
	size_t	byte_size = recv(sock->getSocket(), buffer, RECV_BUFFER_SZ, MSG_DONTWAIT);
	if (byte_size < 0)
		return (POLL_ERR);
	else if (byte_size == 0)
	{
		this->_onQuit(*sock);
		return (POLL_DELETE);
	}
	sock->getRecievedData().addData(buffer);			
	while (!sock->getRecievedData().isInvalid()){
		this->_onRecv(*sock, sock->getRecievedData());
		if (!sock)
			return (POLL_DELETE);
		sock->getRecievedData().flush();
	}
	return (POLL_SUCCESS);
}



t_pollevent		AClient::_pollOutClients(SockStream *const sock)
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



t_pollevent		AClient::_pollFromServers(SockStream *const sock, int event)
{	
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


t_pollevent     AClient::_onPollEvent(int socket, int event)
{
	SockStream  *sock = this->_sockets[socket];
	if (!sock)
		return (POLL_NOTFOUND);
	if (sock->getType() == CLIENT)
		return (this->_pollFromServers(sock, event));
	return (POLL_NOTFOUND);
}



