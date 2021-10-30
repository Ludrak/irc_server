#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "AEntity.hpp"
# include "SockStream.hpp"

class Client;

# include "AServer.hpp"

class Client : public SockStream, public AEntity 
{

	public:

		Client();
		Client(AServer &master);
		Client(SockStream &master);
		Client(int socket, const sockaddr_in &addr);
		Client( Client const & src );
		~Client();

		Client &		operator=( Client const & rhs );
		typedef bool	(*Operations)(Client & client, std::string str);

		AServer			&getServer( void ) const;
	private:
		//socket
		std::string				_nickname;
		std::string				_realname;
		std::map<std::string, Operations>	_op;
		AServer 				*_master;
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */