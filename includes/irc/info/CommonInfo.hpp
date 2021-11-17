#ifndef COMMONINFO_HPP
# define COMMONINFO_HPP

# include <sys/types.h>
# include <string>
# include "Logger.hpp"

class CommonInfo
{
	public:
		CommonInfo(const std::string &uid);

		const std::string	&getUID() const;
		void				setUID( const std::string & uid );

		const std::string&	getName( void ) const;
		void				setName( const std::string &name );

		const std::string&	getPassword( void ) const;
		void				setPassword( const std::string &pass );

	protected:
		std::string			_uid;
		std::string		_name;
		std::string		_password;
};

#endif //COMMONINFO_HPP