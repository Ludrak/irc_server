#ifndef COMMONINFO_HPP
# define COMMONINFO_HPP

# include <sys/types.h>
# include <string>

class CommonInfo
{
	public:
		const std::string&	getName( void ) const;
		const std::string&	getPassword( void ) const;

	protected:
		std::string		_name;
		std::string		_password;
};

#endif //COMMONINFO_HPP