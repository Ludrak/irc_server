#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

class AServer;
class Channel;
class Client;

# include "AServer.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include "IRCProtocol.hpp"

class IRCServer : public AServer
{

	public:

		IRCServer(int port = 8080, const std::string & password = "", const std::string &host = "127.0.0.1");
		virtual ~IRCServer();

		Channel*				getChannel(int ChannelUID);
		const SockStream&		getForwardSocket( void ) const;
		bool					setNetworkConnection(const std::string & host, int port, std::string & password);
	private:
		IRCProtocol		_protocol;

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