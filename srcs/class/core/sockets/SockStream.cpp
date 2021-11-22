#include "SockStream.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

//TODO CHeck return values
SockStream::SockStream(IProtocol & protocol)
:	_type(UNKNOWN),
#ifndef KQUEUE
	_poll_events(POLLIN),
#else
	_kqueue_events(EVFILT_READ),
#endif
	_ip(""),
	_host(""),
	_protocol(&protocol),
	_received_data(protocol)
{
	Logger::debug("default SockStream constructor");
	this->_createSocket("127.0.0.1", 8080);
}

SockStream::SockStream(const std::string &host, uint16_t port, IProtocol & protocol)
:	_type(UNKNOWN),
#ifndef KQUEUE
	_poll_events(POLLIN),
#else
	_kqueue_events(EVFILT_READ),
#endif
	_ip(""),
	_host(""),
	_protocol(&protocol),
	_received_data(protocol)
{
	this->_createSocket(host, port);
}

SockStream::SockStream(ushort socket, const sockaddr_in &address, IProtocol & protocol)
:	_socket(socket),
	_type(UNKNOWN),
#ifndef KQUEUE
	_poll_events(POLLIN),
#else
	_kqueue_events(EVFILT_READ),
#endif
	_addr(address),
	_ip(""),
	_host(""),
	_protocol(&protocol),
	_received_data(protocol)
{
	struct in_addr ipAddr = address.sin_addr;
	char ip_str[INET_ADDRSTRLEN];
	inet_ntop( AF_INET, &ipAddr, ip_str, INET_ADDRSTRLEN );
	this->_resolveIP(ip_str);
	//REVIEW don't we need createSocket() here?
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

SockStream::~SockStream()
{
	this->close();
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void							SockStream::_resolveIP(const std::string &host)
{
	struct hostent *hostent = gethostbyname(host.c_str());
	if (hostent && hostent->h_addr_list && *hostent->h_addr_list)
	{
		this->_ip = inet_ntoa(*((struct in_addr*)hostent->h_addr_list[0]));
		Logger::debug("Resolved IP: " + this->_ip);
		struct hostent *h = gethostbyaddr(*hostent->h_addr_list, hostent->h_length, hostent->h_addrtype);
		if (h)
			this->_host = h->h_name;
		else
			this->_host = this->_ip;
		Logger::debug("resolved host: " + this->_host);
	}
	else
		Logger::error("Unable to resolve host: " + host);
	return ;
}


void							SockStream::_createSocket(const std::string &host, uint16_t port, sa_family_t family, int sock_type)
{
	Logger::debug("create socket");
	if ((this->_socket = socket(family, sock_type, 0)) < 0)
		throw SockStream::SocketCreationException();
	this->_resolveIP(host);
	bzero(reinterpret_cast<void *>(&this->_addr), sizeof(this->_addr));
	this->_addr.sin_port = htons(port);
	this->_addr.sin_family = family;
	this->_addr.sin_addr.s_addr = inet_addr(this->_ip.c_str());
}

void							SockStream::close( void )
{
	::close(this->_socket);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

ushort							SockStream::getSocket( void ) const
{
	return this->_socket;
}

const struct sockaddr_in&		SockStream::getAddress( void ) const
{
	return this->_addr;
}

IProtocol						*SockStream::getProtocol( void ) const
{
	return this->_protocol;
}

#ifndef KQUEUE
int								SockStream::getPollEvents() const 
{
	return (this->_poll_events);
}
void							SockStream::setPollEvent(int event)
{
	this->_poll_events |= event;
}
void							SockStream::delPollEvent(int event)
{
	this->_poll_events &= ~event;
}
#else
int								SockStream::getkQueueEvents() const 
{
	return (this->_kqueue_events);
}
void							SockStream::setkQueueEvents(struct kevent &ev, int event)
{
	//this->_kqueue_events |= event;
	EV_SET(&ev, ev.ident, event, EV_ADD, 0, 0, NULL);
}
void							SockStream::delkQueueEvents(struct kevent &ev, int event)
{
	//this->_kqueue_events &= ~event;
	EV_SET(&ev, ev.ident, event, EV_DISABLE, 0, 0, NULL);
}
#endif

void							SockStream::setPackageProtocol(IProtocol &proto)
{
	this->_protocol = &proto;
}

Package							&SockStream::getReceivedData()
{
	return (this->_received_data);
}

std::list<Package*>				&SockStream::getPendingData()
{
	return (this->_pending_data);
}

t_sock_type						SockStream::getType(void) const
{
	return (this->_type);
}


void							SockStream::setType( const t_sock_type type )
{
	int option = (type == SERVER);
	if (option && (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) != 0
				|| fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0))
		return ;
	this->_type = type;
}

const std::string				&SockStream::getIP(void) const
{
	return (this->_ip);
}

const std::string				&SockStream::getHost(void) const
{
	return (this->_host);
}

/* ************************************************************************** */