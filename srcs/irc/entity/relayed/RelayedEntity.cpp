
#include "RelayedEntity.hpp"

RelayedEntity::RelayedEntity(Server &relay, const int hopcount, const std::string &uid, const uint type)
: AEntity(type | RelayedEntity::value_type, uid), _hopCount(hopcount), _relay(relay)
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
 
Server              &RelayedEntity::getServer()
{
    return (this->_relay);
}
 
