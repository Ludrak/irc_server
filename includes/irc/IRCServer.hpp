#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

class Channel;
class Client;
class CommandHandler;

# include "status.hpp"
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

// Commands
# include "CommandHandler.hpp"
# include "ACommand.hpp"
# include "CommandPass.hpp"
# include "CommandNick.hpp"
# include "CommandUser.hpp"
# include "CommandPrivmsg.hpp"
# include "CommandJoin.hpp"

# define SUCCESS				0

# define IRC_DEFAULT_HOST "127.0.0.1"
# define IRC_DEFAULT_PORT 6667
# define IRC_DEFAULT_PASS ""

class IRCServer : public ANode, public AEntity, public ServerInfo
{
	public:
		static const uint	value_type;

		IRCServer(ushort port = IRC_DEFAULT_PORT, const std::string &password = IRC_DEFAULT_PASS, const std::string &host = IRC_DEFAULT_HOST);
		virtual ~IRCServer();

		bool						connectToNetwork(const std::string & host, ushort port, std::string & password);
		const IProtocol&			getProtocol( void ) const;
		Channel*					getChannel(int ChannelUID);
		NetworkEntity*				getEntityByStream(SockStream & s);

		UnRegisteredConnection*		getUnRegisteredConnectionByUID(const std::string UID);

		bool						alreadyInUseUID(std::string & uid) const; 

		/* prefix parser */
		std::string					makePrefix(const AEntity *user=NULL, const AEntity *host_server=NULL);
		bool						parsePrefix(const std::string &prefix, Server **const sender_server, RelayedClient **const user, RelayedServer **const host_server);

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

		typedef uint	(IRCServer::*Operations)(AEntity & exector, std::string params);

		/* Operations lists */
		CommandHandler				_handler;

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
** ---------------------------- Client Management -----------------------------
*/

		void				_addClient(AEntity &client, UnRegisteredConnection * executor);
		void				_addServer(AEntity &server);
		void				_deleteClient(const std::string &nick);
		void				_deleteServer(const std::string &token);

		AEntity				*_registerClient(AEntity & client, int type);
		void				_registerServer(AEntity & server, int type);
		void				_sendMessage(AEntity & target, const std::stringstream &message, const AEntity *except=NULL);
		void				_sendMessage(AEntity & target, const std::string &message, const AEntity *except=NULL);
		void				_sendMessage(SockStream & target, const std::stringstream &message);
		void				_sendMessage(SockStream & target, const std::string &message);

/*
** ----------------------------------- DBG ---------------------------------
*/
		void				_printServerState( void );
/*
** --------------------------------- COMMANDS ---------------------------------
*/
		void				_initCommands(void);

		/* CommandHandler is friend so that it can access server maps & other method */
		/* even if they're not public, this means that the server fields will still  */
		/* be protected by IRCServer without having to pass a reference to 			 */
		/* CommandHandler -> ACommand -> CommandX to get fields from IRCServer	     */
		friend class CommandHandler;
		/* friend keyword does'nt support inheritance so we have to friend every     */
		/* command class                                                             */
		friend class CommandPass;
		friend class CommandNick;
		friend class CommandUser;
		friend class CommandPrivmsg;
		friend class CommandJoin;

};

#endif /* ******************************************************* IRCSERVER_H */