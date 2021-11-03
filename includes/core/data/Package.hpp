#ifndef PACKAGE_HPP
# define PACKAGE_HPP

class SockStream;

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
		Package( const IProtocol &protocol, const std::string &data="", const SockStream *recipient=NULL ) throw(Package::SizeExeededException, Package::InvalidProtocolException);
		Package( Package const &src );
		virtual ~Package();

		Package&		operator=( Package const & rhs );

		void			addData( const std::string &new_data ) throw(Package::SizeExeededException);
		void			flush( void );
		void			nflush( uint n );

		bool			isInvalid( void ) const;
		std::string		getData( void ) const;
		std::string		getRawData( void ) const;
		SockStream      *getRecipient( void ) const;
		void			setRecipient( const SockStream *recipient );

	private:
		std::string			_data;
		bool				_is_invalid;
		const IProtocol		*_protocol;
		const SockStream	*_recipient;

		bool			_checksum( void );
};

std::ostream &			operator<<( std::ostream & o, Package const & i );

#endif // PACKAGE_HPP