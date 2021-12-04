#include "SockStream.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

//TODO CHeck return values
SockStream::SockStream(const IProtocol & protocol, const bool useTLS)
throw (InvalidHostException, SSLException)
:	_type(UNKNOWN),
#ifdef	POLL
	_poll_events(POLLIN),
#elif	defined(EPOLL)
#elif	defined(SELECT)
#elif	defined(KQUEUE)
	_kqueue_events(EVFILT_READ),
#endif
	_ip("127.0.0.1"),
	_host(""),
	_protocol(&protocol),
	_useTLS(useTLS),
	_cSSL(NULL),
	_received_data(protocol)
{
	Logger::core("default SockStream constructor");
	this->_createSocket("127.0.0.1", 8080);
}

SockStream::SockStream(const std::string &host, uint16_t port, const IProtocol & protocol, const bool useTLS)
throw (InvalidHostException, SSLException)
:	_type(UNKNOWN),
#ifdef	POLL
	_poll_events(POLLIN),
#elif	defined(EPOLL)
#elif	defined(KQUEUE)
	_kqueue_events(EVFILT_READ),
#endif
	_ip(host),
	_host(host),
	_protocol(&protocol),
	_useTLS(useTLS),
	_cSSL(NULL),
	_received_data(protocol)
{
	this->_createSocket(host, port);
}

SockStream::SockStream(ushort socket, const sockaddr_in &address, const IProtocol & protocol, const bool useTLS, SSL_CTX * const ctx)
throw (SSLException)
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
	_useTLS(useTLS),
	_cSSL(NULL),
	_received_data(protocol)
{
	struct in_addr ipAddr = address.sin_addr;
	char ip_str[INET_ADDRSTRLEN];
	inet_ntop( AF_INET, &ipAddr, ip_str, INET_ADDRSTRLEN );
	this->_resolveIP(ip_str);
	if (this->_useTLS)
		this->initTLS(ctx);
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
		Logger::core("Resolved IP: " + this->_ip);
		struct hostent *h = gethostbyaddr(*hostent->h_addr_list, hostent->h_length, hostent->h_addrtype);
		if (h)
			this->_host = h->h_name;
		else
			this->_host = this->_ip;
		Logger::core("resolved host: " + this->_host);
	}
	else
	{
		Logger::error("Unable to resolve host: " + host);
		throw InvalidHostException();
	}
	return ;
}

void							SockStream::_createSocket(const std::string &host, uint16_t port, sa_family_t family, int sock_type)
throw (SockStream::InvalidHostException, SockStream::SSLException)
{
	Logger::core("create socket");
	if ((this->_socket = socket(family, sock_type, 0)) < 0)
		throw SockStream::SocketCreationException();
	this->_resolveIP(host);
	std::memset(reinterpret_cast<void *>(&this->_addr), 0, sizeof(this->_addr));
	this->_addr.sin_port = htons(port);
	this->_addr.sin_family = family;
	this->_addr.sin_addr.s_addr = inet_addr(this->_ip.c_str());
}

void							SockStream::initTLS(SSL_CTX *ctx) throw (SSLException)
{
	if (!ctx)
	{
		Logger::critical("SockStream: Passing null SSL context to SockStream.");
		return ;
	}
	Logger::core("SockStream: Initialising TLS");
	this->_cSSL = SSL_new(ctx);
//	SSL_set_connect_state(this->_cSSL);
//TLS
	SSL_get_read_ahead(this->_cSSL);

	if (!this->_cSSL)
	{
		Logger::error("SockStream: failed to create SSL structure");
		ERR_print_errors_fp(stderr);
		throw SSLException();
	}
	//SSL_set_fd(this->_cSSL, this->_socket);
}

int							SockStream::acceptSSL()
{
	SSL_set_fd(this->_cSSL, this->_socket);
	//TLS
	int ret = SSL_accept(this->_cSSL);
	if (ret == 0)
	{
		/* Hard error */
		Logger::error("Cannot SSL_accept() for client: " + ntos(this->_socket) + "@" + this->_host);
		ERR_print_errors_fp(stderr);
		return -1;
	}
	else if (ret == -1)
	{
		int err = SSL_get_error(this->_cSSL, ret);
		if (err == SSL_ERROR_WANT_READ)
		{
			/* Wait for data to be read */
			Logger::warning("SockStream:  Wait for data to be read ");
			return 0;
		}
		else if (err == SSL_ERROR_WANT_WRITE)
		{
			/* Write data to continue */
			Logger::warning("SockStream: write data to continue ");
			return 0;
		}
		else if (err == SSL_ERROR_SYSCALL || err == SSL_ERROR_SSL || err == SSL_ERROR_ZERO_RETURN)
		{
			/* Hard error */
			Logger::error("Cannot SSL_accept() for client: " + ntos(this->_socket) + "@" + this->_host);
			ERR_print_errors_fp(stderr);
			return -1;
		}
	}
	return 1;
}

void							SockStream::connectSSL()
{
	SSL_set_connect_state(this->_cSSL);
	SSL_set_fd(this->_cSSL, this->_socket);
	int err = SSL_connect(this->_cSSL);
	if (err <= 0)
	{
		Logger::error("Cannot SSL_connect() for client: " + ntos(this->_socket) + "@" + this->_host);
		ERR_print_errors_fp(stderr);
		//Logger::error("Cannot SSL_connect() for client:" + std::string(ERR_error_string(ERR_get_error(), NULL)));
		//this->_cSSL = NULL;
		//this->_useTLS = false;

	}
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

const IProtocol						*SockStream::getProtocol( void ) const
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
	if (option
		&& ((setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) != 0)
		|| (fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0)))
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

void							SockStream::useTLS(const bool use)
{
	this->_useTLS = use;
}

bool							SockStream::hasTLS(void) const
{
	return (this->_useTLS);
}

SSL								*SockStream::getSSL() const
{
	return (this->_cSSL);
}

void							SockStream::addTrafficSize(const uint64_t size)
{
	this->_trafficSize += size;
}

uint64_t						SockStream::getTraffic() const
{
	return (this->_trafficSize);
}


/* ************************************************************************** */