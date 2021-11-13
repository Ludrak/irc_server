
#include "UnRegisteredConnection.hpp"

UnRegisteredConnection::UnRegisteredConnection(SockStream &stream)
: NetworkEntity(stream, "", UnRegisteredConnection::value_type), UnRegisteredConnectionInfo()
{
}
