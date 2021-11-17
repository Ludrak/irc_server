
#include "RelayedServer.hpp"

RelayedServer::RelayedServer(
    Server              &relay,
    const int           hop,
    const std::string   &token,
    const std::string   &name,
    const std::string   &info,
    const std::string   &version,
    const std::string   &host,
    const std::string   &flags
)
:   RelayedEntity(relay, hop, token, RelayedServer::value_type),
    ServerInfo(info, version, host, flags)
{
    this->_name = name;
    this->_family = SERVER_ENTITY_FAMILY;
}
