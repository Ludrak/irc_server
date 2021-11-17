
#ifndef RELAYED_ENTITY
# define RELAYED_ENTITY


#include "AEntity.hpp"
#include "Server.hpp"

class RelayedEntity : public AEntity
{
    public:
        static const uint	value_type = RELAYED_ENTITY_TYPE;
    public:
        RelayedEntity(Server &relay, const int hopcount, const std::string &uid, const uint type);
        virtual ~RelayedEntity();

        int                 getHopCount() const;
        const Server        &getServer() const;

    private:
        uint                _hopCount;
        Server              &_relay;
};

#endif // RELAYED_ENTITY