#ifndef CLIENTINFO_HPP
# define CLIENTINFO_HPP

# include "CommonInfo.hpp"
# include "UnRegisteredConnectionInfo.hpp"

# define NB_CLIENT_REGISTRATION_MAX 12

class ClientInfo : public CommonInfo
{
	public:
		/* relayed client info (when USER is recieved with hopcount > 0) */
		ClientInfo(const std::string &name, const std::string &real_name, const uint &mode, const std::string &pass);

		/* directly connected client info */
		ClientInfo(const UnRegisteredConnectionInfo &reference);
		ClientInfo(const ClientInfo &copy);

		bool				isFull( void ) const;
		uint				getConcurrentChannels( void ) const; 
		uint				getConcurrentChannelsMax( void ) const;

		/* mode */
		void				toogleMode( uint modeMask );
		void				enableMode( uint modeMask );
		void				disableMode( uint modeMask );
		bool				isEnable( uint modeMask ) const;
		uint				getMode() const;

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
	
	private:
		ClientInfo();
};

#endif //CLIENTINFO_HPP