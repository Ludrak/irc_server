#include "SockStream.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

//TODO CHeck return values
SockStream::SockStream(IProtocol & protocol)
throw (InvalidHostException)
:	_type(UNKNOWN),
#ifdef	POLL
	_poll_events(POLLIN),
#elif	defined(EPOLL)
#elif	defined(SELECT)
#elif	defined(KQUEUE)
	_kqueue_events(EVFILT_READ),
#endif
	_ip(""),
	_host(""),
	_protocol(&protocol),
	_recieved_data(protocol)
{
	Logger::debug("default SockStream constructor");
	this->_createSocket("127.0.0.1", 8080);
}

SockStream::SockStream(const std::string &host, uint16_t port, IProtocol & protocol)
throw (InvalidHostException)
:	_type(UNKNOWN),
#ifdef	POLL
	_poll_events(POLLIN),
#elif	defined(EPOLL)
#elif	defined(KQUEUE)
	_kqueue_events(EVFILT_READ),
#endif
	_ip(""),
	_host(""),
	_protocol(&protocol),
	_recieved_data(protocol)
{
	this->_createSocket(host, port);
}

SockStream::SockStream(ushort socket, const sockaddr_in &address, IProtocol & protocol)
:	_socket(socket),
	_type(UNKNOWN),
#ifdef	POLL
	_poll_events(POLLIN),
#elif	defined(EPOLL)
#elif	defined(KQUEUE)
	_kqueue_events(EVFILT_READ),
#endif
	_addr(address),
	_ip(""),
	_host(""),
	_protocol(&protocol),
	_recieved_data(protocol)
{
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

void							SockStream::_createSocket(const std::string &host, uint16_t port, sa_family_t family, int sock_type)
throw (SockStream::InvalidHostException)
{
	if ((this->_socket = socket(family, sock_type, 0)) < 0)
		throw SockStream::SocketCreationException();

	struct hostent *hostent = gethostbyname(host.c_str());
	if (hostent && hostent->h_addr_list && *hostent->h_addr_list)
	{
		this->_ip = std::string(inet_ntoa(*((struct in_addr*)hostent->h_addr_list[0])));
		struct hostent *h = gethostbyaddr(*hostent->h_addr_list, hostent->h_length, hostent->h_addrtype);
		if (h)
			this->_host = h->h_name;
		else 
			this->_host = this->_ip;
	}
	else
		throw InvalidHostException();
	
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

#ifdef	POLL
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
#elif	defined(EPOLL)
int								SockStream::getEPollEvents() const 
{
	return (this->_epoll_events);
}
void							SockStream::setEPollEvent(int event)
{
	this->_epoll_events |= event;
}
void							SockStream::delEPollEvent(int event)
{
	this->_epoll_events &= ~event;
}
#elif 	defined(SELECT)
int								SockStream::getSelectedIOs(void) const
{
	return (this->_selectedIO);
}

void							SockStream::selectIO(int event)
{
	this->_selectedIO |= event;
}

void							SockStream::deselectIO(int event)
{
	this->_selectedIO &= ~event;
}

#elif	defined(KQUEUE)
void							SockStream::setkQueueEvents(int kq, std::vector<struct kevent> ev_list, struct kevent &ev, int event)
{
	//Logger::info("Set event " + ntos(event) + " to " + ntos(ev.ident));
	EV_SET(&ev, ev.ident, event, EV_ADD, 0, 0, NULL);
	struct timespec *n = NULL;
	kevent(kq, ev_list.data(), ev_list.size(), NULL, 0, n);
}
void							SockStream::delkQueueEvents(int kq, std::vector<struct kevent> ev_list, struct kevent &ev, int event)
{
	//Logger::info("Del event " + ntos(event) + " to " + ntos(ev.ident));
	EV_SET(&ev, ev.ident, event, EV_DELETE, 0, 0, NULL);
	struct timespec *n = NULL;
	kevent(kq, ev_list.data(), ev_list.size(), NULL, 0, n);
}
#endif

void							SockStream::setPackageProtocol(IProtocol &proto)
{
	this->_protocol = &proto;
}

Package							&SockStream::getRecievedData()
{
	return (this->_recieved_data);
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