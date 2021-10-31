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

		Package( const IProtocol &protocol, const std::string &data="" )
		throw(Package::SizeExeededException, Package::InvalidProtocolException)
		{
			this->_protocol = &protocol;
			if (!data.empty())
				this->addData(data);
			else
				this->_is_invalid = true;
		}

		Package( Package const & src )
		{
			*this = src;
		}

		virtual ~Package()
		{
		}

		Package&		operator=( Package const & rhs )
		{
			if (this == &rhs)
				return (*this);
			this->_data = rhs._data;
			this->_is_invalid = rhs._is_invalid;
			this->_protocol = rhs._protocol;
			return (*this);
		}

		bool			isInvalid( void ) const
		{
			return (this->_is_invalid);
		}

		void			addData( const std::string &new_data ) throw(Package::SizeExeededException)
		{
			if (this->_data.size() + new_data.size() > this->_protocol->getMaximumPackageSize())
				throw Package::SizeExeededException();
			this->_data += new_data;
			this->_checksum();
		}

		std::string		getData( void ) const
		{
			size_t pk_sz = this->_protocol->isProtocol(this->_data);
			if (pk_sz == 0)
				return (this->_data);
			return (this->_data.substr(0, pk_sz));
		}

		void			flush( void )
		{
			size_t pk_sz = this->_protocol->isProtocol(this->_data);
			if (pk_sz != 0)
				this->_data = this->_data.substr(pk_sz, this->_data.size() - pk_sz);
			this->_checksum();
		}

	private:
		std::string		_data;
		bool			_is_invalid;
		const IProtocol	*_protocol;

		bool			_checksum( void )
		{
			this->_is_invalid = (this->_protocol->isProtocol(this->_data) == 0);
			return (this->_is_invalid);
		}
};

std::ostream &			operator<<( std::ostream & o, Package const & i );

#endif // PACKAGE_HPP