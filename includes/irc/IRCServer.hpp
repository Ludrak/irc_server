#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

class AServer;
class Channel;
class Client;

# include "AServer.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include "IRCProtocol.hpp"

# define IRC_DEFAULT_HOST "127.0.0.1"
# define IRC_DEFAULT_PORT 6667
# define IRC_DEFAULT_PASS ""

class IRCServer : public AServer
{

	public:
		IRCServer(int port = IRC_DEFAULT_PORT, const std::string &password = IRC_DEFAULT_PASS, const std::string &host = IRC_DEFAULT_HOST);
		virtual ~IRCServer();

		Channel*				getChannel(int ChannelUID);
		const SockStream&		getForwardSocket( void ) const;
		bool					setNetworkConnection(const std::string & host, int port, std::string & password);

	private:
		IRCProtocol				_protocol;

		IRCServer( IRCServer const & src );

		IRCServer&				operator=( IRCServer const & rhs );

		std::list<Channel>	 	_channels;
		SockStream				_forward_socket;
		std::string				_password;
		std::string				_network_password;

		void					_onClientJoin(SockStream &s);
		void					_onClientRecv(SockStream &s, const Package pkg);
		void					_onClientQuit(SockStream &s);
};

#endif /* ******************************************************* IRCSERVER_H */