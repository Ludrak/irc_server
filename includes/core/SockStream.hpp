#ifndef SOCKSTREAM_HPP
# define SOCKSTREAM_HPP

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
# include "Package.hpp"

# define RECV_BUFFER_SZ	255
# define SEND_BUFFER_SZ	255

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

		SockStream(IProtocol & protocol);
		SockStream(const std::string &host, uint16_t port, IProtocol & protocol);
		SockStream(int socket, const sockaddr_in &address, IProtocol & protocol);
		~SockStream();

		SockStream &				operator=( SockStream const & rhs );

		void						setPackageProtocol(IProtocol &proto);

		size_t						recieve(size_t read_bufsz=RECV_BUFFER_SZ);
		size_t						send(size_t send_bufsz=SEND_BUFFER_SZ);

		int							getSocket( void ) const;
		const struct sockaddr_in&	getSockaddr( void ) const;
		IProtocol					*getProtocol( void ) const;
		Package						&getRecievedData();
		Package						&getPendingData();

	protected:
		int							_socket;
		struct sockaddr_in			_addr;
		IProtocol					*_protocol;

	private:
		Package				 		_recieved_data;
		Package						_pending_data;

		SockStream( SockStream const & src );

		void						_createSocket(const std::string &host, uint16_t port, sa_family_t familly = AF_INET, int sock_type = SOCK_STREAM);


};

#endif /* ***************************************************** SockStream_H */