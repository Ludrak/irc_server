
#ifndef RELAYED_ENTITY
# define RELAYED_ENTITY

#include "AEntity.hpp"
#include "Server.hpp"

# define RELAYED_ENTITY_TYPE 2

class RelayedEntity : public AEntity
{
    public:
        static const int	value_type;

    public:
        RelayedEntity(Server &relay, const int hopcount, const int type);

        int             getHopCount() const;
        const Server    &getRelayServer() const;

    private:
        int             _hopCount;
        Server          &_relay;
};

const int RelayedEntity::value_type = RELAYED_ENTITY_TYPE;

#endif // RELAYED_ENTITY