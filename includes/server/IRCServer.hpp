#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

# include "AServer.hpp"
# include "Client.hpp"
# include "IRCProtocol.hpp"

class IRCServer : public AServer
{

	public:

		IRCServer();
		virtual ~IRCServer();

		int		getNetworkSocket( void ) const;

	private:
		IRCProtocol		_protocol;

		IRCServer( IRCServer const & src );
		IRCServer &		operator=( IRCServer const & rhs );
		std::list<Channel>	 	_channels;
		int						_Server_network_socket;
		bool					_init_server( void );
		Client					*_acceptConnection(SockStream &socket_client);

		void					_onClientJoin(SockStream &s);
		void					_onClientRecv(SockStream &s, const Package pkg);
		void					_onClientQuit(SockStream &s);
};

std::ostream &			operator<<( std::ostream & o, IRCServer const & i );

#endif /* ******************************************************* IRCSERVER_H */