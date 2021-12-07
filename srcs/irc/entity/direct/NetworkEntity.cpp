
#include "NetworkEntity.hpp"

NetworkEntity::NetworkEntity(SockStream &stream, const std::string &uid, const uint type)
:   AEntity(type | NetworkEntity::value_type, uid),
    _stream(stream),
	_cleanDisconnection(false)
{
}

NetworkEntity::~NetworkEntity()
{
}

const SockStream	&NetworkEntity::getStream() const
{
    return (this->_stream);
}

SockStream      	&NetworkEntity::getStream()
{
    return (this->_stream);
}

bool				NetworkEntity::isCleanDisconnection( void ) const
{
	return this->_cleanDisconnection;
}

void				NetworkEntity::setCleanDisconnection( const bool clean )
{
	this->_cleanDisconnection = clean;
}
