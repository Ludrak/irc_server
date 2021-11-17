
#include "RelayedClient.hpp"

RelayedClient::RelayedClient(
    Server            &relay,
    const int         hopcount,
    const std::string &nick,
    const std::string &name,
    const std::string &real_name,
    const uint        mode,
    const std::string &server_token,
    const std::string &server_host
)
:   RelayedEntity(relay, hopcount, nick, RelayedClient::value_type),
    ClientInfo(real_name, mode, server_token, server_host)
{
    this->_name = name;
    this->_family = CLIENT_ENTITY_FAMILY;
}