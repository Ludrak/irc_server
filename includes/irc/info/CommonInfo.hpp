#ifndef COMMONINFO_HPP
# define COMMONINFO_HPP

# include <string>

class CommonInfo
{
	public:
		const std::string&	getName( void ) const;
		const std::string&	getUid( void ) const;
		const std::string&	getPassword( void ) const;

	protected:
		std::string		_name;
		std::string		_uid;
		std::string		_password;
}

#endif //COMMONINFO_HPP