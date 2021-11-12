# include "ServerInfo.hpp"

const std::string&			ClientInfo::getInfo( void ) const
{
	return this->_info;
}

const std::string&			ClientInfo::getVersion( void ) const
{
	return this->_version;
}

const std::string&			ClientInfo::getFlags( void ) const
{
	return this->_flags;
}
