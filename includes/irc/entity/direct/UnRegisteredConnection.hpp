
#ifndef UNREGISTERED_CONNECTION
# define UNREGISTERED_CONNECTION

#include "NetworkEntity.hpp"
#include "UnRegisteredConnectionInfo.hpp"

class UnRegisteredConnection : public NetworkEntity, public UnRegisteredConnectionInfo
{
    public:
        static const uint value_type = UNREGISTERED_CONNECTION_ENTITY_TYPE;

    public:
        UnRegisteredConnection(SockStream &stream);
};

#endif // UNREGISTERED_CONNECTION