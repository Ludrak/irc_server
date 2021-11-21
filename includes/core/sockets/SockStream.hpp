#ifndef SOCKSTREAM_HPP
# define SOCKSTREAM_HPP

class AServer;
class SockStream;

# include <iostream>
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
/* KQUEUE implemented: need minor fixes */
/* EPOLL not implmented: not availaible on macos */
/* SELECT not yet implemented */
//# define POLL
//# define EPOLL
//# define KQUEUE
# define SELECT

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
	
	
	public:
		/* server socket constructors */
		SockStream(IProtocol &protocol) throw (InvalidHostException);
		SockStream(const std::string &host, uint16_t port, IProtocol &protocol) throw (InvalidHostException);
		/* client socket contructor */
		SockStream(ushort socket, const sockaddr_in &address, IProtocol &protocol);
		
		~SockStream();

		SockStream &				operator=( SockStream const &rhs );

		void						close(void);
		void						setPackageProtocol(IProtocol &proto);

		ushort						getSocket(void) const;
		const struct sockaddr_in&	getAddress(void) const;
		IProtocol					*getProtocol(void) const;
		Package						&getRecievedData(void);
		std::list<Package*>			&getPendingData(void);

#ifdef	POLL
		int							getPollEvents(void) const;
		void						setPollEvent(int event);
		void						delPollEvent(int event);
#elif	defined(EPOLL)
		int							getEPollEvents(void) const;
		void						setEPollEvent(int event);
		void						delEPollEvent(int event);
#elif	defined(SELECT)
# define SELECT_IO_WRITE	(1<<1)
# define SELECT_IO_READ		(1<<2)
		int							getSelectedIOs(void) const;
		void						selectIO(int event);
		void						deselectIO(int event);
#elif	defined(KQUEUE)
		void						setkQueueEvents(struct kevent &ev, int event);
		void						delkQueueEvents(struct kevent &ev, int event);
#endif

		t_sock_type					getType(void) const;
		void						setType( const t_sock_type type );

		const std::string			&getIP(void) const;		
		const std::string			&getHost(void) const;

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
		IProtocol					*_protocol;

	private:
		Package				 		_recieved_data;
		std::list<Package*>			_pending_data;

		SockStream( SockStream const & src );

		void						_createSocket(const std::string &host, uint16_t port,
										sa_family_t familly = AF_INET, int sock_type = SOCK_STREAM)
										throw(SockStream::InvalidHostException);
};

#endif /* ***************************************************** SockStream_H */
