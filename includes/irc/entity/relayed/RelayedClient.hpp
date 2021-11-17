
#ifndef RELAYED_CLIENT_HPP
# define RELAYED_CLIENT_HPP

#include "RelayedEntity.hpp"
#include "ClientInfo.hpp"

class RelayedClient : public RelayedEntity, public ClientInfo
{
	public:
		static const uint	value_type = RELAYED_CLIENT_ENTITY_TYPE;
    
    public:
		RelayedClient(
			Server            &relay,
			const int         hopcount,
			const std::string &nick,
			const std::string &name,
			const std::string &real_name,
			const uint        mode,
			const std::string &server_token,
			const std::string &server_host
		);
};

#endif // RELAYED_CLIENT_HPP