# include "ClientInfo.hpp"

ClientInfo::ClientInfo(const std::string &nickname, const std::string &name, const std::string &real_name, const uint &mode)
: CommonInfo()
{
	this->_uid = nickname;
	this->_name = name;
	this->_realname = real_name;
	this->_mode = mode;
}

ClientInfo::ClientInfo(const UnRegisteredConnectionInfo &reference)
: CommonInfo(),	_mode(0), _concurrentChannels(0), _concurrentChannelsMax(NB_CLIENT_REGISTRATION_MAX),
_realname("default real name"), _serverToken("420"), _host("127.0.0.1"), _privilege(0)
{
	this->_uid = reference.getUID();
	this->_name = reference.getName();
	this->_password = reference.getPassword();
}

ClientInfo::ClientInfo(const ClientInfo &copy)
: CommonInfo(),	_mode(copy.getMode()), _concurrentChannels(copy.getConcurrentChannels()), _concurrentChannelsMax(NB_CLIENT_REGISTRATION_MAX),
_realname(copy.getRealname()), _serverToken(copy.getServerToken()), _host(copy.getHostname()), _privilege(copy.isPrivilegied())
{
	this->_uid = copy.getUID();
	this->_name = copy.getName();
	this->_password = copy.getPassword();
}

bool						ClientInfo::isFull( void ) const
{
	return (this->_concurrentChannels == this->_concurrentChannelsMax);
}

uint						ClientInfo::getConcurrentChannels( void ) const
{
	return this->_concurrentChannels;
}

uint						ClientInfo::getConcurrentChannelsMax( void ) const
{
	return this->_concurrentChannelsMax;
}

/* mode */
void						ClientInfo::toogleMode(uint modeMask)
{
	this->_mode ^= modeMask;
}

void						ClientInfo::enableMode(uint modeMask)
{
	this->_mode |= modeMask;
}

void						ClientInfo::disableMode(uint modeMask)
{
	this->_mode &= ~modeMask;
}

bool						ClientInfo::isEnable(uint modeMask) const
{
	return (this->_mode & modeMask);	
}

uint						ClientInfo::getMode() const
{
	return (this->_mode);
}

const std::string&			ClientInfo::getRealname( void ) const
{
	return this->_realname;
}

const std::string&			ClientInfo::getServerToken( void ) const
{
	return this->_serverToken;
}

const std::string&			ClientInfo::getHostname( void ) const
{
	return this->_host;
}

bool						ClientInfo::isPrivilegied( void ) const
{
	return (this->_privilege > 0);
}

