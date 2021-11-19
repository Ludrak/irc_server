
#ifndef RELAYED_SERVER_HPP
# define RELAYED_SERVER_HPP

#include "RelayedEntity.hpp"
#include "ServerInfo.hpp"

class RelayedServer : public RelayedEntity, public ServerInfo
{
	public:
		static const uint		value_type = RELAYED_SERVER_ENTITY_TYPE;

		public:
			RelayedServer(
				Server              &relay,
				const int           hop,
				const std::string   &token,
				const std::string   &name,
				const std::string   &info,
				const std::string   &host
			);
};


#endif // RELAYED_SERVER_HPP