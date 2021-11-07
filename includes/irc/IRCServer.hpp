#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

class AServer;
class Channel;
class Client;

# include "Channel.hpp"
# include "Client.hpp"
# include "Parser.hpp"
# include "Logger.hpp"
# include "ntos.hpp"
# include "AIrcClient.hpp"
# include "IRCProtocol.hpp"
# include "ANode.hpp"

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

class IRCServer : public ANode
{

	public:
		IRCServer(int port = IRC_DEFAULT_PORT, const std::string &password = IRC_DEFAULT_PASS, const std::string &host = IRC_DEFAULT_HOST);
		virtual ~IRCServer();

		bool					setNetworkConnection(const std::string & host, int port, std::string & password);
		const IProtocol&		getProtocol( void ) const;
		Channel*				getChannel(int ChannelUID);
		AIrcClient&				getClientBySockStream(SockStream & s);

	
	private:

		typedef uint	(IRCServer::*UserOperations)(Client & client, std::string str);
		typedef uint	(IRCServer::*ServerOperations)(AIrcClient & client, std::string str);
	
		std::map<std::string, AIrcClient*>		_ircClients;
		std::list<AIrcClient*>					_pendingConnections;
		std::map<std::string, UserOperations>	_userCommands;
		std::map<std::string, ServerOperations>	_serverCommands;
		//network
		int										_forwardSocket;
		std::string								_password;
		std::string								_networkSocket;
		IRCProtocol								_protocol;

/*
** --------------------------------- EVENTS ---------------------------------
*/
		void					_onClientJoin( SockStream &s );
		void					_onClientRecv( SockStream &s, Package &pkg );
		void					_onClientQuit( SockStream &s );
		void					_onRecv( SockStream &server,  Package &pkg );
		void					_onConnect ( SockStream &server );
		void					_onQuit( SockStream &server );
		void					_onKicked( SockStream &server );

/*
** --------------------------------- SocketAction ---------------------------------
*/

	void						_setRegistered(AIrcClient & client);
	void						_sendMessage(AIrcClient & client, std::string message, uint error = 0);
		
/*
** --------------------------------- DEBUG ---------------------------------
*/
		void					_printServerState( void );
/*
** --------------------------------- COMMANDS ---------------------------------
*/
		void		_init_commands(void);
		int			execute(AIrcClient & client, std::string data);
		uint		userCommandPass(Client & client, std::string cmd);
		uint		userCommandNick(Client & client, std::string cmd);
		uint		userCommandUser(Client & client, std::string cmd);
		uint		userCommandPrivmsg(Client & client, std::string cmd);
		uint		userCommandDescribe(Client & client, std::string cmd);

		uint		serverCommandNick(Client & client, std::string cmd);
};

#endif /* ******************************************************* IRCSERVER_H */