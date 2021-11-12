#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

class Channel;
class Client;

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
		IRCServer(ushort port = IRC_DEFAULT_PORT, const std::string &password = IRC_DEFAULT_PASS, const std::string &host = IRC_DEFAULT_HOST);
		virtual ~IRCServer();

		bool					setNetworkConnection(const std::string & host, ushort port, std::string & password);
		const IProtocol&		getProtocol( void ) const;
		Channel*				getChannel(int ChannelUID);
		Client*					getClientBySockStream(SockStream & s);

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

	void						_setRegistered(Client & client, int type);
	void						_sendMessage(AEntity & client, const std::stringstream &message);
	void						_sendMessage(SockStream & target, const std::stringstream &message);
		
/*
** --------------------------------- DEBUG ---------------------------------
*/
		void					_printServerState( void );
/*
** --------------------------------- COMMANDS ---------------------------------
*/
		void		_init_commands(void);
		int			execute(AEntity & executor, std::string data);
		uint		_commandPASS(Client & client, std::string cmd);
		uint		_commandNICK(Client & client, std::string cmd);
		uint		_commandUSER(Client & client, std::string cmd);
		uint		_commandSERVER(Client & client, std::string cmd);
		uint		_commandPRIVMSG(Client & client, std::string cmd);
		uint		_commandDESCRIBE(Client & client, std::string cmd);

};

#endif /* ******************************************************* IRCSERVER_H */