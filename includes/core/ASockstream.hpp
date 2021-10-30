#ifndef ASockStream_HPP
# define ASockStream_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <exception>
# include <unistd.h>

class ASockStream
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

		ASockStream();
		ASockStream(const std::string &host, uint16_t port);
		ASockStream(int socket, const sockaddr_in &address);
		ASockStream( ASockStream const & src );
		~ASockStream();

		ASockStream &		operator=( ASockStream const & rhs );

		protected:
			int						_socket;
			struct sockaddr_in		_addr;		
	private:
		void					_createSocket(const std::string &host, uint16_t port, sa_family_t familly = AF_INET, int sock_type = SOCK_STREAM);


};

std::ostream &			operator<<( std::ostream & o, ASockStream const & i );

#endif /* ***************************************************** ASockStream_H */