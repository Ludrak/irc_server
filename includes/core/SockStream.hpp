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
# include <poll.h>
# include <list>
# include "Package.hpp"

# define RECV_BUFFER_SZ	255
# define SEND_BUFFER_SZ	255


typedef uint8_t	t_sock_type;
# define SERVER	1
# define CLIENT	2

class SockStream
{
	class SocketCreationException : public std::exception
	{
		virtual const char	*what() const throw()
		{
			return ("Socket creation failed: ");
		}
	};

	public:

		SockStream(IProtocol &protocol);
		SockStream(const std::string &host, uint16_t port, IProtocol &protocol);
		SockStream(int socket, const sockaddr_in &address, IProtocol &protocol);
		~SockStream();

		SockStream &				operator=( SockStream const &rhs );

		void						close(void);
		void						setPackageProtocol(IProtocol &proto);

		int							getSocket(void) const;
		const struct sockaddr_in&	getSockaddr(void) const;
		IProtocol					*getProtocol(void) const;
		Package						&getRecievedData(void);
		std::list<Package*>			&getPendingData(void);

		int							getPollEvents(void) const;
		void						setPollEvent(int event);
		void						delPollEvent(int event);

		t_sock_type					getType(void) const;

	protected:
		int							_socket;
		t_sock_type					_type;
		int							_poll_events;
		struct sockaddr_in			_addr;
		IProtocol					*_protocol;

	private:
		Package				 		_recieved_data;
		std::list<Package*>			_pending_data;

		SockStream( SockStream const & src );

		void						_createSocket(const std::string &host, uint16_t port, sa_family_t familly = AF_INET, int sock_type = SOCK_STREAM);
};

#endif /* ***************************************************** SockStream_H */