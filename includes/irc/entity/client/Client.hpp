#ifndef CLIENT_HPP
# define CLIENT_HPP

class IRCServer;

# include "AEntity.hpp"
# include "IRCServer.hpp"
# include "AClient.hpp"

class Client : public AClient
{

	public:

		Client(IRCServer &master, SockStream & socket);
		virtual ~Client();

		Client &			operator=( Client const & rhs );

		uint				getId( void ) const;
		
		enum C { value_type = 1 };
	
	private:

		Client( Client const & src );
		std::string							_nickname;
		std::string							_realname;
		AServer*							_master;
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */