
#include "RelayedServer.hpp"

RelayedServer::RelayedServer(Server &relay, const int hop, const std::string token, const std::string name, const std::string info)
: RelayedEntity(relay, hop, RelayedServer::value_type), ServerInfo(token, name, info)
{
}
