#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP

# include "CommonInfo.hpp"
# include "UnRegisteredConnectionInfo.hpp"

class ServerInfo : public CommonInfo
{
	public:
		/* relayed server constructor */
		ServerInfo(const std::string &token, const std::string &servname, const std::string &servinfo);
		
		/* directly connected server constructor */
		ServerInfo(const UnRegisteredConnectionInfo &reference);
		ServerInfo(const ServerInfo &copy);

		const std::string&	getInfo( void ) const;
		const std::string&	getVersion( void ) const;
		const std::string&	getFlags( void ) const;

	protected:
		std::string			_info;
		std::string			_version;
		std::string			_flags;
};

#endif //SERVERINFO_HPP