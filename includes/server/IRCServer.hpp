#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

class AServer;
class Channel;
class Client;

# include "AServer.hpp"
# include "Channel.hpp"
# include "Client.hpp"

class IRCServer : public AServer
{

	public:

		IRCServer();
		~IRCServer();

		int					getNetworkSocket( void ) const;
		Channel				*getChannel(int		ChannelId);

	private:

		IRCServer( IRCServer const & src );

		IRCServer&				operator=( IRCServer const & rhs );

		std::list<Channel>	 	_channels;
		int						_Server_network_socket;
		bool					_init_server( void );
		Client					*_acceptConnection(SockStream &socket_client);
};

#endif /* ******************************************************* IRCSERVER_H */