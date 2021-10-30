#ifndef PACKAGE_HPP
# define PACKAGE_HPP

# include <iostream>
# include <string>

template<typename T>
class Package
{
	class PackageSizeExeededException : public std::exception
	{
		virtual const char      *what( void ) const throw()
		{
			return ("Package size exeeded");
		}
	};

	public:
		Package() : _data(""), _is_invalid(true)
		{
		}

		Package( const std::string &data ) throw(Package::PackageSizeExeededException)
		{
			this->addData(data);
			this->_checksum();
		}

		Package( Package const & src )
		{
			*this = src;
		}

		~Package()
		{
		}

		Package&		operator=( Package const & rhs )
		{
			if (this == &rhs)
				return (*this);
			this->_data = rhs.getData();
			this->_is_invalid = rhs.isInvalid();
		}

		bool			isInvalid( void ) const
		{
			return (this->_is_invalid);
		}

		void			addData( const std::string &new_data ) throw (Package::PackageSizeExeededException)
		{
			if (this->_data.size() + new_data.size() > T::getMaximumPackageSize())
				throw Package<T>::PackageSizeExeededException();
			this->_data += new_data;
			this->_is_invalid = this->_checksum();
		}

		std::string		getData( ) const
		{
			return (this->_data);
		}

	private:
		std::string		_data;
		bool			_is_invalid;

		bool			_checksum( void )
		{
			this->_is_invalid = T::isProtocol(this->_data);
			return (this->_is_invalid);
		}
};

template<typename T>
std::ostream &			operator<<( std::ostream & o, Package<T> const & i );

#endif // PACKAGE_HPP