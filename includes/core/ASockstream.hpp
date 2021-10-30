#ifndef ASOCKSTREAM_HPP
# define ASOCKSTREAM_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <fcntl.h>
# include <arpa/inet.h>
class ASockstream
{

	public:

		ASockstream();
		ASockstream( ASockstream const & src );
		~ASockstream();

		ASockstream &		operator=( ASockstream const & rhs );

	private:
		int						_socket;
		int						_socket_familly;
		int						_socket_type;
		struct sockaddr_in		_serv_addr;
};

std::ostream &			operator<<( std::ostream & o, ASockstream const & i );

#endif /* ***************************************************** ASOCKSTREAM_H */