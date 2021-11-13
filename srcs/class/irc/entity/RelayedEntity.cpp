
#include "RelayedEntity.hpp"

RelayedEntity::RelayedEntity(Server &relay, const int hopcount, const int type) : AEntity(type), _relay(relay), _hopCount(hopcount)
{
}

