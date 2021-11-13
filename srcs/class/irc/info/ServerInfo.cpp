# include "ServerInfo.hpp"

ServerInfo::ServerInfo(const std::string &name, const std::string &info, const std::string &pass)
: CommonInfo()
{
	this->_name = name;
	this->_info = info;
	this->_password = pass;
}

ServerInfo::ServerInfo(const UnRegisteredConnectionInfo &reference)
: CommonInfo(), _info(""), _version("1"), _flags("")
{
	this->_name = reference.getName();
	this->_password = reference.getPassword();
}

ServerInfo::ServerInfo(const ServerInfo &copy)
: CommonInfo(), _info(copy.getInfo()), _version(copy.getVersion()), _flags(copy.getFlags())
{
	this->_name = copy.getName();
	this->_password = copy.getPassword();
}


const std::string&			ServerInfo::getInfo( void ) const
{
	return this->_info;
}

const std::string&			ServerInfo::getVersion( void ) const
{
	return this->_version;
}

const std::string&			ServerInfo::getFlags( void ) const
{
	return this->_flags;
}
