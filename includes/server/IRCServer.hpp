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

		IRCServer(int port = 8080, const std::string & password = "", const std::string &host = "127.0.0.1");
		~IRCServer();

		Channel*				getChannel(int ChannelUID);
		const SockStream&		getForwardSocket( void ) const;
		bool					setNetworkConnection(const std::string & host, int port, std::string & password);
	private:

		IRCServer( IRCServer const & src );

		IRCServer&				operator=( IRCServer const & rhs );

		std::list<Channel>	 	_channels;
		SockStream				_forward_socket;
		bool					_init_server( void );
		std::string				_password;
		std::string				_network_password;
		Client					*_acceptConnection(SockStream &socket_client);
};

#endif /* ******************************************************* IRCSERVER_H */