
#include "NetworkEntity.hpp"

NetworkEntity::NetworkEntity(SockStream &stream, const int type) : AEntity(type), _stream(stream)
{
}

SockStream  &NetworkEntity::getStream() const
{
    return (this->_stream);
}