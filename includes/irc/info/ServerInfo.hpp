#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP

# include "CommonInfo.hpp"
# include "UnRegisteredConnectionInfo.hpp"

class ServerInfo
{
	public:
		/* relayed server constructor */
		/* created by command which contains all info about the new server */
		ServerInfo(
			const std::string	&info,
			const std::string	&version,
			const std::string	&host,
			const std::string	&flags
		);

		const std::string&	getInfo( void ) const;
		void				setInfo( const std::string &info );

		const std::string&	getVersion( void ) const;
		void				setVersion( const std::string &version );

		const std::string&	getFlags( void ) const;
		void				setFlags( const std::string &flags );

		const std::string&	getHostname( void ) const;
		void				setHostname( const std::string &host );

	protected:
		std::string			_info;
		std::string			_version;
		std::string			_host;
		std::string			_flags;
};

#endif //SERVERINFO_HPP