#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP

# include "CommonInfo.hpp"

class ServerInfo : public CommonInfo
{
	public:
		const std::string&	getInfo( void ) const;
		const std::string&	getVersion( void ) const;
		const std::string&	getFlags( void ) const;

	protected:
		std::string			_info;
		std::string			_version;
		std::string			_flags;
}

#endif //SERVERINFO_HPP