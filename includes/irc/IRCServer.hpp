#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

class Channel;
class Client;

# include "StatusCode.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include "Parser.hpp"
# include "Logger.hpp"
# include "ntos.hpp"
# include "IRCProtocol.hpp"
# include "ANode.hpp"

# define IRC_DEFAULT_HOST "127.0.0.1"
# define IRC_DEFAULT_PORT 6667
# define IRC_DEFAULT_PASS ""

class IRCServer : public ANode
{

	public:
		IRCServer(ushort port = IRC_DEFAULT_PORT, const std::string &password = IRC_DEFAULT_PASS, const std::string &host = IRC_DEFAULT_HOST);
		virtual ~IRCServer();

		bool								setNetworkConnection(const std::string & host, ushort port, std::string & password);
		const IProtocol&					getProtocol( void ) const;
		Channel*							getChannel(int ChannelUID);
		Client*								getClientBySockStream(SockStream & s);

		static std::string					statusMessages[MAX_STATUS_MESSAGES + 1];
	
	private:

		typedef uint	(IRCServer::*UserOperations)(Client & client, std::string str);
		typedef uint	(IRCServer::*ServerOperations)(AEntity & client, std::string str);
	
		ushort									_forwardSocket;
		std::string								_password;
		std::map<std::string, AEntity*>			_ircClients;
		std::list<Client*>						_pendingConnections;
		std::map<std::string, UserOperations>	_userCommands;
		std::map<std::string, ServerOperations>	_serverCommands;
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

/*
** --------------------------------- SocketAction ---------------------------------
*/

	bool						_reply(Client & client, ushort statusCode, std::string target = "", std::string target2 = "");
	void						_setRegistered(Client & client);
	void						_sendMessage(AEntity & client, std::string message, SockStream * except = NULL);
		
/*
** --------------------------------- DEBUG ---------------------------------
*/
		void					_printServerState( void );
/*
** --------------------------------- COMMANDS ---------------------------------
*/
		void					_initCommands(void);
		void					_initStatusMessages( void );

		int						execute(AEntity & client, std::string data);
		uint					_commandPASS(Client & client, std::string cmd);
		uint					_commandNICK(Client & client, std::string cmd);
		uint					_commandUSER(Client & client, std::string cmd);
		uint					_commandPRIVMSG(Client & client, std::string cmd);
		uint					_commandDESCRIBE(Client & client, std::string cmd);
		uint					_commandJOIN(Client & client, std::string cmd);
		uint					_commandMODE(Client & client, std::string cmd);
		uint					_commandMODEclient(Client & client, std::string cmd); //Not use this in array of methods
		uint					_commandMODEchannel(Client & client, Channel& target, size_t nbParam, std::string cmd);//Not use this in array of methods

};

#endif /* ******************************************************* IRCSERVER_H */