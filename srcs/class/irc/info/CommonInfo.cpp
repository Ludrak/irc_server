# include "CommonInfo.hpp"

const std::string&		CommonInfo::getName( void ) const
{
	return this->_name;
}

const std::string&		CommonInfo::getUID( void ) const
{
	return this->_uid;
}

const std::string&		CommonInfo::getPassword( void ) const
{
	return this->_password;
}