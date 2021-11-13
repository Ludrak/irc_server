
#include "RelayedClient.hpp"

RelayedClient::RelayedClient(Server &relay, const int hopcount, const std::string &nick, const std::string &name, const std::string &real_name, const uint mode)
: RelayedEntity(relay, hopcount, RelayedClient::value_type), ClientInfo(nick, name, real_name, mode)
{
}