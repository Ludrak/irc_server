
#include "NetworkEntity.hpp"

NetworkEntity::NetworkEntity(SockStream &stream, const std::string &uid, const uint type)
:   AEntity(type | NetworkEntity::value_type, uid),
    _stream(stream)
{
}

NetworkEntity::~NetworkEntity()
{
}

const SockStream  &NetworkEntity::getStream() const
{
    return (this->_stream);
}

SockStream        &NetworkEntity::getStream()
{
    return (this->_stream);
}

