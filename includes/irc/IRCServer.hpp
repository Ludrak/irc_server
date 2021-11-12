#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

class Channel;
class Client;

# include "StatusCode.hpp"
# include <string>
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
		const std::string		&getName() const;
		const std::string		&getInfo() const;
		uint					getToken() const;
		void					setName(const std::string &name);
		void					setInfo(const std::string &info);
		void					setToken(const uint token);
	
	private:

		typedef uint	(IRCServer::*Operations)(Client & client, std::string str);

		ushort									_forwardSocket;
		std::string								_password;
		std::map<std::string, AEntity*>			_ircClients;
		std::list<Client*>						_pendingConnections;
		std::map<std::string, Operations>		_userCommands;
		std::map<std::string, Operations>		_serverCommands;
		std::map<std::string, Operations>		_unregisteredCommands;
		IRCProtocol								_protocol;

		/* server infos */
		std::string								_name;
		std::string								_info;
		uint									_token;

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
	void						_setRegistered(Client & client, int type);
void							_sendMessage(AEntity & target, const std::stringstream &message, const AEntity *except=NULL);
	void						_sendMessage(SockStream & target, const std::stringstream &message);
		
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
		uint					_commandJOIN(Client & client, std::string cmd);
		uint					_commandMODE(Client & client, std::string cmd);
		uint					_commandMODEclient(Client & client, std::string cmd); //Not use this in array of methods
		uint					_commandMODEchannel(Client & client, Channel& target, size_t nbParam, std::string cmd);//Not use this in array of methods
		uint					_commandSERVER(Client & client, std::string cmd);

};

#endif /* ******************************************************* IRCSERVER_H */