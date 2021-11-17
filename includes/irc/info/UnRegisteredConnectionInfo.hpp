#ifndef UNREGISTEREDCONNECTIONINFO_HPP
# define UNREGISTEREDCONNECTIONINFO_HPP

# include "CommonInfo.hpp"

class UnRegisteredConnectionInfo
{
	public:
		const std::string&	getVersion( void ) const;
		void				setVersion( const std::string & version );
		
		const std::string&	getFlags( void ) const;
		void				setFlags( const std::string & flags );

	protected:
		std::string			_version;
		std::string			_flags;
};

#endif //UNREGISTEREDCONNECTIONINFO_HPP