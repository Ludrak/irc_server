#ifndef CLIENTINFO_HPP
# define CLIENTINFO_HPP

class IRCServer;
# include "AEntity.hpp"
# include <vector>
# include <list>
# include "UnRegisteredConnectionInfo.hpp"
# include "Logger.hpp"

# define NB_CLIENT_REGISTRATION_MAX 12


/* User Modes */
/* ! currently only MODE_OPERATOR is implemented ! */
// TODO update masks to fit USER mask
# define MODE_INVISIBLE 		(1 << 0)
# define MODE_RECVSERVNOTICE	(1 << 1)
# define MODE_WALLOPS			(1 << 2)
# define MODE_OPERATOR			(1 << 3)
# define MODE_AWAY				(1 << 4)
# define MODE_RESTRICTED		(1 << 5)

//need Channel
class Channel;

class ClientInfo
{
	public:
		/* relayed client info (when USER is received with hopcount > 0) */
		/* created by command which contains all info about the new client */
		ClientInfo(
			IRCServer			&server_reference,
			const std::string	&real_name,
			const uint			mode,
			const std::string	&server_token,
			const std::string	&server_host
		);
		
		/* channels */
		bool						maxChannelAccessReached( void ) const;
		uint						getConcurrentChannels( void ) const; 
		uint						getConcurrentChannelsMax( void ) const;
		bool						incrementJoinedChannels( void );
		bool						decrementJoinedChannels( void );

		/* mode */
		uint						getModeBit( char c ) const;
		uint						getMode( void ) const;
		std::string					getModeString( void );
		bool						hasMode( uint modeMask ) const;
		// 1 is on 0 or < is off
		void						setMode( uint modeMask, int value);
		void						toggleMode( uint modeMask );
		void						enableMode( uint modeMask );
		void						disableMode( uint modeMask );

		const std::string&			getRealname( void ) const;
		void						setRealname( const std::string &real_name );

		const std::string&			getServerToken( void ) const;
		void						setServerToken( const std::string &server_token );

		const std::string&			getHostname( void ) const;
		void						setHostname( const std::string &host );

		bool						isServerOP( void ) const;

		IRCServer					&getServerReference(void);

		const std::list<Channel*>	&getChannels() const;
		int 						joinChannel(Channel &channel);
		void                        leaveAllChannels(const std::string &info="");
		virtual void                leaveChannel(Channel &channel, const std::string &info="");


		virtual const std::string	getPrefix( void ) const = 0;

	protected:
		IRCServer					&_serverReference;
		uint						_mode;
		std::string					_realname;
		std::string					_serverToken;
		std::string					_host;
		uint						_concurrentChannels;
		uint						_concurrentChannelsMax;
		std::list<Channel*>			_channels;


	private:
		ClientInfo();
};

# include "Channel.hpp"

#endif //CLIENTINFO_HPP