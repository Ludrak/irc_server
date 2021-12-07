# include "CommonInfo.hpp"

CommonInfo::CommonInfo(const std::string &uid)
: _uid(uid), _name(""), _password("")
{
}

const std::string&		CommonInfo::getName( void ) const
{
	return this->_name;
}

void					CommonInfo::setName( const std::string &name )
{
	this->_name = name;
}

const std::string&		CommonInfo::getPassword( void ) const
{
	return this->_password;
}

void					CommonInfo::setPassword( const std::string &pass )
{
	this->_password = pass;
}

const std::string&		CommonInfo::getUID( void ) const
{
	return this->_uid;
}

void					CommonInfo::setUID( const std::string & uid )
{
	if (uid.empty())
	{
		Logger::critical("set uid with empty value");
		return;
	}
	this->_uid = uid;
}

