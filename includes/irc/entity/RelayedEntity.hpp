
#ifndef RELAYED_ENTITY
# define RELAYED_ENTITY

#include "AEntity.hpp"
#include "Server.hpp"

class RelayedEntity : public AEntity
{
    public:
        static const int	value_type;

    public:
        RelayedEntity(Server &relay, const int hopcount, const std::string &uid, const int type);
        virtual ~RelayedEntity();

        int             getHopCount() const;
        const Server    &getRelayServer() const;

    private:
        int             _hopCount;
        Server          &_relay;
};

const int RelayedEntity::value_type = RELAYED_ENTITY_TYPE;

#endif // RELAYED_ENTITY