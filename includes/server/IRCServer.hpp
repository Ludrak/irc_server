#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

# include "AServer.hpp"
# include "Client.hpp"

class IRCServer : public AServer
{

	public:

		IRCServer();
		~IRCServer();

		int		getNetworkSocket( void ) const;
	private:
		IRCServer( IRCServer const & src );
		IRCServer &		operator=( IRCServer const & rhs );
		std::list<Channel>	 	_channels;
		int						_Server_network_socket;
		bool					_init_server( void );
		Client					*_acceptConnection(SockStream &socket_client);
};

std::ostream &			operator<<( std::ostream & o, IRCServer const & i );

#endif /* ******************************************************* IRCSERVER_H */