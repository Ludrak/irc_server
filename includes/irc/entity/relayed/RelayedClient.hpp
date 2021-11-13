
#ifndef RELAYED_CLIENT_HPP
# define RELAYED_CLIENT_HPP

#include "RelayedEntity.hpp"
#include "ClientInfo.hpp"

# define RELAYED_CLIENT_ENTITY_TYPE 6

class RelayedClient : public RelayedEntity, public ClientInfo
{
	public:
		static const int	value_type;
    
    public:
        RelayedClient(Server &relay, const int hop, const std::string &nick, const std::string &name, const std::string &real_name, const uint mode, const std::string &password);
};

const int RelayedClient::value_type = RELAYED_CLIENT_ENTITY_TYPE;

#endif // RELAYED_CLIENT_HPP