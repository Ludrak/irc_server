# include "UnRegisteredConnectionInfo.hpp"

const std::string&		UnRegisteredConnectionInfo::getVersion( void ) const
{
	return this->_version;
}

void					UnRegisteredConnectionInfo::setVersion( const std::string & version )
{
	this->_version = version;
}


const std::string&		UnRegisteredConnectionInfo::getFlags( void ) const
{
	return this->_flags;
}

void					UnRegisteredConnectionInfo::setFlags( const std::string & flags )
{
	this->_flags = flags;
}
