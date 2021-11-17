# include "ServerInfo.hpp"

ServerInfo::ServerInfo(
	const std::string	&info,
	const std::string	&version,
	const std::string	&host,
	const std::string	&flags
)
:	_info(info),
	_version(version),
	_host(host),
	_flags(flags)
{
}

const std::string&			ServerInfo::getInfo( void ) const
{
	return this->_info;
}

void						ServerInfo::setInfo( const std::string &info )
{
	this->_info = info;	
}

const std::string&			ServerInfo::getVersion( void ) const
{
	return this->_version;
}

void						ServerInfo::setVersion( const std::string &version )
{
	this->_version = version;
}

const std::string&			ServerInfo::getFlags( void ) const
{
	return this->_flags;
}

void						ServerInfo::setFlags( const std::string &flags )
{
	this->_flags = flags;
}

const std::string&			ServerInfo::getHostname( void ) const
{
	return this->_host;
}

void						ServerInfo::setHostname( const std::string &host )
{
	this->_host = host;
}
