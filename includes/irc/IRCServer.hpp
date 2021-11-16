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

# define SUCCESS				0

# define IRC_DEFAULT_HOST "127.0.0.1"
# define IRC_DEFAULT_PORT 6667
# define IRC_DEFAULT_PASS ""

class IRCServer : public ANode, public AEntity, public ServerInfo
{

	public:
		IRCServer(ushort port = IRC_DEFAULT_PORT, const std::string &password = IRC_DEFAULT_PASS, const std::string &host = IRC_DEFAULT_HOST);
		virtual ~IRCServer();

		//bool				setNetworkConnection(const std::string & host, ushort port, std::string & password);
		const IProtocol&	getProtocol( void ) const;
		Channel*			getChannel(int ChannelUID);
		AEntity*			getEntityByStream(SockStream & s);

		/* prefix parser */
		std::string			makePrefix(const AEntity *user=NULL, const AEntity *host_server=NULL);
		bool				parsePrefix(const std::string &prefix, AEntity **const sender_server, AEntity **const user=NULL, AEntity **const host_server=NULL);

		static const int	value_type;
		static std::string	statusMessages[MAX_STATUS_MESSAGES + 1];

	private:
/*
** ------------------------------ ENTITIES ---------------------------------
*/

		/* map of all registered entities of the server */
		std::map<std::string, AEntity*>                 _entities;
		
		/* map of Channels */
		std::map<std::string, Channel*>                 _channels;

		/* map of both Client and RelayedClients */
		std::map<std::string, AEntity*>              	_clients;
		/* map of both Server and RelayedServer */
		std::map<std::string, AEntity*>              	_servers;
		/* map of yet unregistered connections */
		std::map<SockStream*, UnRegisteredConnection*>	_unregistered_connections;
	
		/* map of all direct connections that we can recv on */
		std::map<SockStream*, NetworkEntity*>			_connections;

/*
** ------------------------------ OPERATIONS ---------------------------------
*/

		typedef uint	(IRCServer::*Operations)(NetworkEntity & exector, std::string params);

		/* Operations lists */
		std::map<std::string, Operations>				_userCommands;
		std::map<std::string, Operations>				_serverCommands;
		std::map<std::string, Operations>				_unregisteredCommands;

/*
** ------------------------------- PROTOCOL(S) -------------------------------
*/

		/* protocol for data transmission */
		IRCProtocol										_protocol;

/*
** --------------------------------- EVENTS ---------------------------------
*/
		void				_onClientJoin( SockStream &s );
		void				_onClientRecv( SockStream &s, Package &pkg );
		void				_onClientQuit( SockStream &s );
		void				_onRecv( SockStream &server,  Package &pkg );
		void				_onConnect ( SockStream &server );
		void				_onQuit( SockStream &server );

/*
** --------------------------------- SocketAction ---------------------------------
*/

		bool				_reply(Client & client, ushort statusCode, std::string target = "", std::string target2 = "");
		void				_registerClient(AEntity & client, int type);
		void				_registerServer(AEntity & server, int type);
		void				_sendMessage(AEntity & target, const std::stringstream &message, const AEntity *except=NULL);
		void				_sendMessage(SockStream & target, const std::stringstream &message);
		
/*
** --------------------------------- DEBUG ---------------------------------
*/
		void				_printServerState( void );
/*
** --------------------------------- COMMANDS ---------------------------------
*/
		void				_initCommands(void);
		void				_initStatusMessages( void );

		int					execute(AEntity *entity, std::string data);
		uint				_commandPASS(NetworkEntity & executor, std::string params);
		uint				_commandNICK(NetworkEntity & executor, std::string params);
		uint				_commandUSER(NetworkEntity & executor, std::string params);
		uint				_commandPRIVMSG(NetworkEntity & executor, std::string params);
		uint				_commandJOIN(NetworkEntity & executor, std::string params);
		uint				_commandMODE(NetworkEntity & executor, std::string params);
		uint				_commandSERVER(NetworkEntity & executor, std::string params);

		uint				_commandMODEclient(NetworkEntity & executor, std::string params); //Not use this in array of methods
		uint				_commandMODEchannel(NetworkEntity & executor, Channel& target, size_t nbParam, std::string params);//Not use this in array of methods

};

#endif /* ******************************************************* IRCSERVER_H */