
#include "RelayedClient.hpp"

RelayedClient::RelayedClient(
    IRCServer         &server_reference,
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
    ClientInfo(server_reference, real_name, mode, server_token, server_host)
{
    this->_name = name;
    this->_family = CLIENT_ENTITY_FAMILY;
}
const std::string			RelayedClient::getIdent( void ) const
{
	return (this->_uid + "!" + this->_name + "@" + this->_host);
}

const std::string			RelayedClient::getPrefix( void ) const
{
	return ":" + this->getIdent() + " ";
}