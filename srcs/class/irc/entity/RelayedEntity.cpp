
#include "RelayedEntity.hpp"

RelayedEntity::RelayedEntity(Server &relay, const int hopcount, const std::string &uid, const int type)
: AEntity(type, uid), _relay(relay), _hopCount(hopcount)
{
}

RelayedEntity::~RelayedEntity()
{

}

