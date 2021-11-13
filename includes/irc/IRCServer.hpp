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
# include "ServerInfo.hpp"
# include "ClientInfo.hpp"
# include "RelayedClient.hpp"
# include "RelayedServer.hpp"

# define IRC_DEFAULT_HOST "127.0.0.1"
# define IRC_DEFAULT_PORT 6667
# define IRC_DEFAULT_PASS ""

class IRCServer : public ANode, public ServerInfo
{

	public:
		IRCServer(ushort port = IRC_DEFAULT_PORT, const std::string &password = IRC_DEFAULT_PASS, const std::string &host = IRC_DEFAULT_HOST);
		virtual ~IRCServer();

		//bool								setNetworkConnection(const std::string & host, ushort port, std::string & password);
		const IProtocol&					getProtocol( void ) const;
		Channel*							getChannel(int ChannelUID);
		AEntity*							getEntityByStream(SockStream & s);

		static std::string					statusMessages[MAX_STATUS_MESSAGES + 1];

	private:

		typedef uint	(IRCServer::*Operations)(Client & client, std::string str);
		
		/* entities */
		//std::map<std::string, AEntity*>			_ircClients;
		//std::list<Client*>						_pendingConnections;

		/* list of all registered entities of the server */
		std::map<std::string, AEntity*>                 _entities;
		/* list of Channels */
		std::map<std::string, Channel*>                 _channels;
		/* list of both Client and RelayedClients */
		std::map<std::string, ClientInfo*>              _clients;
		/* list of both Server and RelayedServer */
		std::map<std::string, ServerInfo*>              _servers;
		/* list of yet unregistered connections */
		std::map<SockStream*, UnRegisteredConnection*>	_unregistered_connections;
		/* list of all direct connections that we can recv on */
		std::map<SockStream*, NetworkEntity*>			_connections;

		// global entities  known (?)
		// std::vector<SockStream*>						_known_entities
		
		/* Operations */
		std::map<std::string, Operations>				_userCommands;
		std::map<std::string, Operations>				_serverCommands;
		std::map<std::string, Operations>				_unregisteredCommands;

		/* protocol for data transmission */
		IRCProtocol										_protocol;

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
	void						_registerClient(AEntity & client, int type);
	void						_sendMessage(AEntity & target, const std::stringstream &message, const AEntity *except=NULL);
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

		int						execute(AEntity *entity, std::string data);
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