#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

class AServer;
class Channel;
class Client;

# include "AServer.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include "AClient.hpp"
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
		AClient&				getClientBySockStream(SockStream & s);
		bool					setNetworkConnection(const std::string & host, int port, std::string & password);

		typedef bool	(IRCServer::*UserOperations)(Client & client, std::string str);
		typedef bool	(IRCServer::*ServerOperations)(AClient & client, std::string str);
	private:

		IRCServer( IRCServer const & src );
		IRCServer&				operator=( IRCServer const & rhs );

		IRCProtocol								_protocol;
		std::map<std::string, AClient*>			_ircClients;
		std::list<AClient*>						_pendingConnections;
		std::map<std::string, UserOperations>	_userCommands;
		std::map<std::string, ServerOperations>	_serverCommands;
		// std::map<std::string, Operations>	_opCommands;

		SockStream				_forward_socket;
		std::string				_password;
		std::string				_networkSocket;

		void					_onClientJoin(SockStream &s);
		void					_onClientRecv(SockStream &s, Package &pkg);
		void					_onClientQuit(SockStream &s);
/*
** --------------------------------- COMMANDS ---------------------------------
*/
		void		_init_commands(void);
		int			execute(AClient & client, std::string data);
		bool		userCommandUser(Client & client, std::string cmd);
		bool		userCommandPass(Client & client, std::string cmd);
		bool		userCommandNick(Client & client, std::string cmd);
};

#endif /* ******************************************************* IRCSERVER_H */