
#ifndef UNREGISTERED_CONNECTION
# define UNREGISTERED_CONNECTION

#include "NetworkEntity.hpp"
#include "UnRegisteredConnectionInfo.hpp"

class UnRegisteredConnection : public NetworkEntity, public UnRegisteredConnectionInfo
{
    public:
        static const uint value_type;

    public:
        UnRegisteredConnection(SockStream &stream);
};

const uint UnRegisteredConnection::value_type = UNREGISTERED_CONNECTION_ENTITY_TYPE;

#endif // UNREGISTERED_CONNECTION