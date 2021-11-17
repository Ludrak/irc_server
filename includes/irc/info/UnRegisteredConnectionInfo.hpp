#ifndef UNREGISTEREDCONNECTIONINFO_HPP
# define UNREGISTEREDCONNECTIONINFO_HPP

# include "CommonInfo.hpp"

class UnRegisteredConnectionInfo
{
	public:
		const std::string&	getVersion( void ) const;

	protected:
		std::string			_version;
};

#endif //UNREGISTEREDCONNECTIONINFO_HPP