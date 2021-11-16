
#include "RelayedClient.hpp"

RelayedClient::RelayedClient(Server &relay, const int hopcount, const std::string &nick, const std::string &name, const std::string &real_name, const uint mode, const std::string &pass)
: RelayedEntity(relay, hopcount, nick, RelayedClient::value_type), ClientInfo(name, real_name, mode, pass)
{
    this->_family = CLIENT_ENTITY_FAMILY;
}