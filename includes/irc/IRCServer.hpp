#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

class Channel;
class Client;
class CommandHandler;

# include <string>
# include <ctime>
# include "status.hpp"
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
# include "NetworkEntity.hpp"

// Commands
# include "CommandHandler.hpp"
# include "CommandPass.hpp"
# include "CommandNick.hpp"
# include "CommandUser.hpp"
# include "CommandPrivmsg.hpp"
# include "CommandJoin.hpp"
# include "CommandServer.hpp"
# include "CommandError.hpp"
# include "CommandMode.hpp"
# include "CommandQuit.hpp"
# include "CommandSquit.hpp"
# include "CommandOper.hpp"
# include "CommandMotd.hpp"
# include "CommandConnect.hpp"
# include "CommandDie.hpp"
# include "CommandPong.hpp"
# include "CommandPart.hpp"
# include "CommandVersion.hpp"
# include "CommandKill.hpp"
# include "CommandNotice.hpp"
# include "CommandNjoin.hpp"
# include "CommandTopic.hpp"
# include "CommandList.hpp"
# include "CommandKick.hpp"

# define SUCCESS				0

# define IRC_DEFAULT_MOTD_PATH	"./conf/"
# define IRC_DEFAULT_HOST		"127.0.0.1"
# define IRC_DEFAULT_PORT		6667
# define IRC_DEFAULT_TLS_PORT	6697
# define IRC_DEFAULT_PASS		""
# define IRC_CURRENT_VERSION	"021O"

class IRCServer : public ANode, public AEntity, public ServerInfo
{
	public:
		class InitialisationException : public std::exception
		{
			virtual const char*	what(void ) const throw ()
			{
				return "IRCserver initialisation failed. (see log for more informations)";	
			}
		};

		static const uint	value_type;

		IRCServer(ushort port = IRC_DEFAULT_PORT,
					const std::string &password = IRC_DEFAULT_PASS,
					const std::string &host = IRC_DEFAULT_HOST,
					const std::string &ssl_cert_path="",
					const std::string &ssl_key_path="",
					const ushort tls_port=IRC_DEFAULT_TLS_PORT );
		virtual ~IRCServer();

		bool						connectToNetwork(const std::string & host, ushort port, std::string & password, const bool useTLS=false);
		const IProtocol&			getProtocol( void ) const;
		NetworkEntity*				getEntityByStream(SockStream & s);
		UnRegisteredConnection*		getUnRegisteredConnectionByUID(const std::string UID);
		std::string					getMotdsPath( void ) const;

		void						setDebugLevel( bool debug);
		uint						getDebugLevel( void ) const;
		bool						alreadyInUseUID(std::string & uid) const; 

		/* prefix parser */
		// bool						parsePrefix(const std::string &prefix, Server **const sender_server, RelayedClient **const user, RelayedServer **const host_server);
		bool						parsePrefix(NetworkEntity & excutor, const std::string &prefix,  RelayedServer **const host_server, AEntity **const user, std::string *username);

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
		CommandHandler							_handler;

/*
** ------------------------------- PROTOCOL(S) -------------------------------
*/

		/* protocol for data transmission */
		IRCProtocol					_protocol;

/*
** ------------------------------- STATE -------------------------------
*/
		void						_forgetLocalServer(Server & srv);

		std::string					_forwardPassword;
		std::string					_operName;
		std::string					_operPassword;
		bool						_shortMotdEnabled;
		bool						_debugLevel;

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
		void				_addServer(AEntity &server, UnRegisteredConnection * executor);

		AEntity				*_registerClient(AEntity & client, int type);
		void				_registerServer(AEntity & server, int type);
		void				_sendMessage(AEntity & target, const std::stringstream &message, AEntity *except=NULL);
		void				_sendMessage(AEntity & target, const std::string &message, AEntity *except=NULL);
		void				_sendMessage(SockStream & target, const std::stringstream &message);
		void				_sendMessage(SockStream & target, const std::string &message);
		
		void				_sendAllClients(const std::string &message, AEntity *except=NULL);
		void				_sendAllServers(const std::string &message, AEntity *except=NULL);

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
		friend class CommandServer;
		friend class CommandError;
		friend class CommandMode;
		friend class CommandQuit;
		friend class CommandSquit;
		friend class CommandOper;
		friend class CommandMotd;
		friend class CommandConnect;
		friend class CommandDie;
		friend class CommandPong;
		friend class CommandPart;
		friend class CommandVersion;
		friend class CommandKill;
		friend class CommandNotice;
		friend class CommandNjoin;
		friend class CommandTopic;
		friend class CommandList;
		friend class CommandKick;

		/* so that clients have a reference to the server they're on */
		friend class ClientInfo;
};

#endif /* ******************************************************* IRCSERVER_H */