#ifndef ASockStream_HPP
# define ASockStream_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <fcntl.h>
# include <arpa/inet.h>
class ASockStream
{

	public:

		ASockStream();
		ASockStream( ASockStream const & src );
		~ASockStream();

		ASockStream &		operator=( ASockStream const & rhs );

		protected:
		
	private:
		int						_socket;
		int						_socket_familly;
		int						_socket_type;
		struct sockaddr_in		_serv_addr;
};

std::ostream &			operator<<( std::ostream & o, ASockStream const & i );

#endif /* ***************************************************** ASockStream_H */