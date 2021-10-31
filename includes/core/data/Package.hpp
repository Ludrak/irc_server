#ifndef PACKAGE_HPP
# define PACKAGE_HPP

# include <iostream>
# include <string>
# include "Protocol.hpp"

class Package
{
	class SizeExeededException : public std::exception
	{
		virtual const char      *what( void ) const throw()
		{
			return ("Package size exeeded");
		}
	};

	class InvalidProtocolException : public std::exception
	{
		virtual const char      *what( void ) const throw()
		{
			return ("Invalid protocol");
		}
	};

	public:
		Package( const IProtocol &protocol, const std::string &data="" ) throw(Package::SizeExeededException, Package::InvalidProtocolException);
		Package( Package const & src );
		virtual ~Package();

		Package&		operator=( Package const & rhs );

		void			addData( const std::string &new_data ) throw(Package::SizeExeededException);
		void			flush( void );

		bool			isInvalid( void ) const;
		std::string		getData( void ) const;

	private:
		std::string		_data;
		bool			_is_invalid;
		const IProtocol	*_protocol;

		bool			_checksum( void );
};

std::ostream &			operator<<( std::ostream & o, Package const & i );

#endif // PACKAGE_HPP