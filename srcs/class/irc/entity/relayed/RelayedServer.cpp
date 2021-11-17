
#include "RelayedServer.hpp"

RelayedServer::RelayedServer(Server &relay, const int hop, const std::string token, const std::string name, const std::string info, const std::string &pass)
: RelayedEntity(relay, hop, token, RelayedServer::value_type), ServerInfo(name, info, pass)
{
    this->_family = SERVER_ENTITY_FAMILY;
}
