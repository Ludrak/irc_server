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
# include "Parser.hpp"

# define IRC_DEFAULT_HOST "127.0.0.1"
# define IRC_DEFAULT_PORT 6667
# define IRC_DEFAULT_PASS ""

# define SUCCESS				0
# define ERR_KICK				1
# define ERR_NOSUCHNICK			401 //"<nickname> :No such nick/channel"
# define ERR_NORECIPENT			411 //":No recipient given (<command>)"
# define ERR_NOTEXTTOSEND		412 //":No text to send"
# define ERR_NONICKNAMEGIVEN	431
# define ERR_ERRONEUSNICKNAME	432
# define ERR_NICKNAMEINUSE		433
# define ERR_NICKCOLLISION		436
# define ERR_NEEDMOREPARAMS		461
# define ERR_ALREADYREGISTRED	462

class IRCServer : public AServer
{

	public:
		IRCServer(int port = IRC_DEFAULT_PORT, const std::string &password = IRC_DEFAULT_PASS, const std::string &host = IRC_DEFAULT_HOST);
		virtual ~IRCServer();

		Channel*				getChannel(int ChannelUID);
		const SockStream&		getForwardSocket( void ) const;
		AClient&				getClientBySockStream(SockStream & s);
		bool					setNetworkConnection(const std::string & host, int port, std::string & password);

		typedef uint	(IRCServer::*UserOperations)(Client & client, std::string str);
		typedef uint	(IRCServer::*ServerOperations)(AClient & client, std::string str);
	private:

		IRCServer( IRCServer const & src );
		IRCServer&				operator=( IRCServer const & rhs );

		std::map<std::string, AClient*>			_ircClients;
		std::list<AClient*>						_pendingConnections;
		std::map<std::string, UserOperations>	_userCommands;
		std::map<std::string, ServerOperations>	_serverCommands;
		// std::map<std::string, Operations>	_opCommands;
		//network
		SockStream								_forwardSocket;
		std::string								_password;
		std::string								_networkSocket;

		//events
		void					_onClientJoin(SockStream &s);
		void					_onClientRecv(SockStream &s, Package &pkg);
		void					_onClientQuit(SockStream &s);
		
		void					setRegistered(AClient & client);
		void					sendMessage(AClient & client, std::string message, uint error = 0);
		void					printServerState( void );

/*
** --------------------------------- COMMANDS ---------------------------------
*/
		void		_init_commands(void);
		int			execute(AClient & client, std::string data);
		uint		userCommandPass(Client & client, std::string cmd);
		uint		userCommandNick(Client & client, std::string cmd);
		uint		userCommandUser(Client & client, std::string cmd);
		uint		userCommandPrivmsg(Client & client, std::string cmd);
		uint		userCommandDescribe(Client & client, std::string cmd);

		uint		serverCommandNick(Client & client, std::string cmd);
};

#endif /* ******************************************************* IRCSERVER_H */