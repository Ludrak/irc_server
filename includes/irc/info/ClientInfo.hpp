#ifndef CLIENTINFO_HPP
# define CLIENTINFO_HPP

# include "CommonInfo.hpp"

class ClientInfo : public CommonInfo
{
	public:
		bool				isFull( void ) const;
		uint				getConcurrentChannels( void ) const; //REVIEW rename? 
		uint				getConcurrentChannelsMax( void ) const; //REVIEW rename?
		

		/* mode */
		void				toogleMode( uint modeMask );
		void				enableMode( uint modeMask );
		void				disableMode( uint modeMask );
		bool				isEnable( uint modeMask ) const;

		const std::string&	getRealname( void ) const;
		const std::string&	getServerToken( void ) const;
		const std::string&	getHostname( void ) const;
		bool				isPrivilegied( void ) const;
		//TODO add inc et dec //REVIEW not here

	protected:
		uint			_mode;
		uint			_concurrentChannels;
		uint			_concurrentChannelsMax;
		std::string		_realname;
		std::string		_serverToken;
		std::string		_host;
		uint			_privilege;
}

#endif //CLIENTINFO_HPP