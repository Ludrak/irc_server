#ifndef SOCKSTREAM_HPP
# define SOCKSTREAM_HPP

class AServer;
class SockStream;

# include <openssl/ssl.h>
# include <openssl/bio.h>
# include <openssl/err.h>
# include <openssl/crypto.h>

# include <iostream>
# include <cstring>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <exception>
# include <vector>
# include <unistd.h>
# include <sys/types.h>
# include <list>
# include <netdb.h>

# include "Package.hpp"
# include "Logger.hpp"
# include "ntos.hpp"

/* POLL implemented: working fine but eating up all CPU usage */
/* KQUEUE implemented: works fine on bsd and macos, prefer EPOLL on Linux  */
/* EPOLL not implemented: not availaible on macos */
/* SELECT implemented: not yet fixed */
//# define POLL
//# define EPOLL
//# define KQUEUE
//# define SELECT

/* Use SELECT if no default sepcified */
#if !defined(SELECT) && !defined(KQUEUE) && !defined(EPOLL) && !defined(POLL)
# define SELECT
#endif

/*
# if defined(__linux__) && defined(EPOLL)
#  include <sys/epoll.h>
# elif defined(KQUEUE) || defined(SELECT) && (defined(__unix__) || defined(BSD) || defined(__APPLE__))
#  if defined(KQUEUE) 
#   include <sys/event.h>
#  elif defined(SELECT)
#   include <sys/select.h>
#  endif
# elif defined(POLL)
#	include <poll.h>
# endif
*/

#if !defined(POLL) && !defined(SELECT) && !defined(KQUEUE)
# define POLL
#endif


#if		defined(POLL)
# include <poll.h>
#elif	defined(SELECT)
# include <sys/select.h>
#elif	defined(KQUEUE)
# include <sys/event.h>
#else
# error No poll-system defined, compile with -D <POLL/KQUEUE/SELECT>
#endif

# define RECV_BUFFER_SZ	255
# define SEND_BUFFER_SZ	255

/** socket types **/
typedef uint8_t	t_sock_type;
# define UNKNOWN		0
/* server sockets in which we are listening */
# define SERVER			1
/* clients that were accepted by server */
# define REMOTE_CLIENT	2
/* clients sockets that we've connected to an host:port */
# define CLIENT			3

class SockStream
{
	class InvalidHostException : public std::exception
	{
		public:
			virtual const char	*what() const throw()
			{
				return ("Invalid hostname");
			}
	};

	class SocketCreationException : public std::exception
	{
		public:
			virtual const char	*what() const throw()
			{
				return ("unable to create socket");
			}
	};
	
	class SSLException : public std::exception
	{
		public:
			virtual const char	*what() const throw()
			{
				return ("failed to instantiate SSL.");
			}
	};
	
	public:
		/* server socket constructors */
		SockStream(const IProtocol &protocol, const bool useTLS=false) throw (InvalidHostException, SSLException);
		SockStream(const std::string &host, uint16_t port, const IProtocol &protocol, const bool useTLS=false) throw (InvalidHostException, SSLException);
		/* client socket contructor */
		SockStream(ushort socket, const sockaddr_in &address, const IProtocol &protocol, const bool useTLS=false, SSL_CTX * const ctx=NULL) throw(SSLException);
		
		~SockStream();

		SockStream &				operator=( SockStream const &rhs );

		void						close(void);
		void						setPackageProtocol(IProtocol &proto);

		ushort						getSocket(void) const;
		const struct sockaddr_in&	getAddress(void) const;
		const IProtocol				*getProtocol(void) const;
		Package						&getReceivedData(void);
		std::list<Package*>			&getPendingData(void);

# ifdef	POLL
		int							getPollEvents(void) const;
		void						setPollEvent(int event);
		void						delPollEvent(int event);
# elif	defined(EPOLL)
		int							getEPollEvents(void) const;
		void						setEPollEvent(int event);
		void						delEPollEvent(int event);
# elif	defined(SELECT)
#  define SELECT_IO_WRITE	(1<<1)
#  define SELECT_IO_READ		(1<<2)
		int							getSelectedIOs(void) const;
		void						selectIO(int event);
		void						deselectIO(int event);
# elif	defined(KQUEUE)
		void						setkQueueEvents(int kq, std::vector<struct kevent> ev_list, struct kevent &ev, int event);
		void						delkQueueEvents(int kq, std::vector<struct kevent> ev_list, struct kevent &ev, int event);
# endif

		t_sock_type					getType(void) const;
		void						setType( const t_sock_type type );

		const std::string			&getIP(void) const;		
		const std::string			&getHost(void) const;

		void						useTLS(const bool use);
		bool						hasTLS(void) const;
		void						initTLS(SSL_CTX *ctx) throw (SockStream::SSLException);
		
		SSL							*getSSL() const;
		void						acceptSSL();
		void						connectSSL();

	protected:
		ushort						_socket;
		t_sock_type					_type;
# ifdef POLL
		int							_poll_events;
# elif  defined(EPOLL)
		int							_epoll_events;
# elif  defined(SELECT)
		int							_selectedIO;
# elif 	defined(KQUEUE)
		int							_kqueue_events;
#endif
		struct sockaddr_in			_addr;
		std::string					_ip;
		std::string					_host;
		const IProtocol				*_protocol;
		bool						_useTLS;
		SSL							*_cSSL;

	private:
		Package				 		_received_data;
		std::list<Package*>			_pending_data;

		SockStream( SockStream const & src );

		void						_resolveIP(const std::string &host);
		void						_createSocket(const std::string &host, uint16_t port,
										sa_family_t familly = AF_INET, int sock_type = SOCK_STREAM)
										throw(SockStream::InvalidHostException, SockStream::SSLException);
};

#endif /* ***************************************************** SockStream_H */
