
#include "NetworkEntity.hpp"

NetworkEntity::NetworkEntity(SockStream &stream, const std::string &uid, const int type) : AEntity(type | NetworkEntity::value_type, uid), _stream(stream)
{
}

NetworkEntity::~NetworkEntity()
{
}

SockStream  &NetworkEntity::getStream() const
{
    return (this->_stream);
}