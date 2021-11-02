#ifndef CLIENT_HPP
# define CLIENT_HPP

class IRCServer;

# include "SockStream.hpp"
# include "AEntity.hpp"
# include "IRCServer.hpp"

class Client : public SockStream, public AEntity 
{

	public:

		Client(IProtocol & protocol);
		Client(IRCServer &master);
		Client(SockStream &master);
		Client(int socket, const sockaddr_in &addr, IProtocol & protocol);
		~Client();

		Client &		operator=( Client const & rhs );
		typedef bool	(Client::*Operations)(Client & client, std::string str);

		IRCServer			&getServer( void ) const;

	private:

		Client( Client const & src );

		IProtocol				*_protocol;
		std::string				_nickname;
		std::string				_realname;
		std::map<std::string, Operations>	_op;
		AServer 							*_master;
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */