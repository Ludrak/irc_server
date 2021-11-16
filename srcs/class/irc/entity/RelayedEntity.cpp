
#include "RelayedEntity.hpp"

RelayedEntity::RelayedEntity(Server &relay, const int hopcount, const std::string &uid, const uint type)
: AEntity(type | RelayedEntity::value_type, uid), _relay(relay), _hopCount(hopcount)
{

}

RelayedEntity::~RelayedEntity()
{

}

int                 RelayedEntity::getHopCount() const
{
    return (this->_hopCount);
}

const Server        &RelayedEntity::getServer() const
{
    return (this->_relay);
}


