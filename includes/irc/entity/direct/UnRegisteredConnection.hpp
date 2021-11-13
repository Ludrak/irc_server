
#ifndef UNREGISTERED_CONNECTION
# define UNREGISTERED_CONNECTION

#include "NetworkEntity.hpp"
#include "UnRegisteredConnectionInfo.hpp"

# define UNREGISTERED_CONNECTION_ENTITY_TYPE 9

class UnRegisteredConnection : public NetworkEntity, public UnRegisteredConnectionInfo
{
    public:
        static const int value_type;
};

const int UnRegisteredConnection::value_type = UNREGISTERED_CONNECTION_ENTITY_TYPE;

#endif // UNREGISTERED_CONNECTION