#ifndef CLIENTINFO_HPP
# define CLIENTINFO_HPP

# include "CommonInfo.hpp"
# include "UnRegisteredConnectionInfo.hpp"
# include "Logger.hpp"

# define NB_CLIENT_REGISTRATION_MAX 12

class ClientInfo
{
	public:
		/* relayed client info (when USER is received with hopcount > 0) */
		/* created by command which contains all info about the new client */
		ClientInfo(
			const std::string	&real_name,
			const uint			mode,
			const std::string	&server_token,
			const std::string	&server_host
		);
		
		/* channels */
		bool				maxChannelAccessReached( void ) const;
		uint				getConcurrentChannels( void ) const; 
		uint				getConcurrentChannelsMax( void ) const;
		bool				incrementJoinedChannels( void );
		bool				decrementJoinedChannels( void );

		/* mode */
		uint				getMode( void ) const;
		std::string			getModeString( void );
		bool				isEnable( uint modeMask ) const;
		void				toogleMode( uint modeMask );
		void				enableMode( uint modeMask );
		void				disableMode( uint modeMask );

		const std::string&	getRealname( void ) const;
		void				setRealname( const std::string &real_name );

		const std::string&	getServerToken( void ) const;
		void				setServerToken( const std::string &server_token );

		const std::string&	getHostname( void ) const;
		void				setHostname( const std::string &host );

		bool				isServerOP( void ) const;
		void				setServerOP( const bool op );

	protected:
		uint			_mode;
		std::string		_realname;
		std::string		_serverToken;
		std::string		_host;
		uint			_serverOp;
		uint			_concurrentChannels;
		uint			_concurrentChannelsMax;
	
	private:
		ClientInfo();
};

#endif //CLIENTINFO_HPP