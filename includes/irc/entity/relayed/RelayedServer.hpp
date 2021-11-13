
#ifndef RELAYED_SERVER_HPP
# define RELAYED_SERVER_HPP

#include "RelayedEntity.hpp"
#include "ServerInfo.hpp"

# define RELAYED_SERVER_ENTITY_TYPE 7

class RelayedServer : RelayedEntity, ServerInfo
{
	public:
		static const int	value_type;

    public:
        RelayedServer(Server &relay, const int hop, const std::string token, const std::string name, const std::string info);
};

const int RelayedServer::value_type = RELAYED_SERVER_ENTITY_TYPE;

#endif // RELAYED_SERVER_HPP