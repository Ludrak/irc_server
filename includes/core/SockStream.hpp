#ifndef SockStream_HPP
# define SockStream_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <exception>
# include <unistd.h>

class SockStream
{
	class SocketCreationException : public std::exception
	{
		virtual const char	*what() const throw()
		{
			//TODO implement errno error
			return "Socket creation failed";
		}
	};

	public:

		SockStream();
		SockStream(const std::string &host, uint16_t port);
		SockStream(int socket, const sockaddr_in &address);
		~SockStream();

		SockStream &				operator=( SockStream const & rhs );
		int							getSocket( void ) const;
		const struct sockaddr_in&	getSockaddr( void ) const;
	protected:
		int						_socket;
		struct sockaddr_in		_addr;
	private:
		SockStream( SockStream const & src );
		void					_createSocket(const std::string &host, uint16_t port, sa_family_t familly = AF_INET, int sock_type = SOCK_STREAM);


};

std::ostream &			operator<<( std::ostream & o, SockStream const & i );

#endif /* ***************************************************** SockStream_H */