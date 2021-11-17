
#include "UnRegisteredConnection.hpp"

UnRegisteredConnection::UnRegisteredConnection(SockStream &stream)
: NetworkEntity(stream, "", UnRegisteredConnection::value_type), UnRegisteredConnectionInfo()
{
}

const std::string&	getVersion( void ) const;
{
	return this->_version;
}

void				setVersion( const std::string & version );
{
	this->_version = version;
}


const std::string&	getFlags( void ) const
{
	return this->_flags;
}

void				setFlags( const std::string & flags )
{
	this->_flags = flags;
}
