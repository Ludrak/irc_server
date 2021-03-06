
#ifndef RELAYED_CLIENT_HPP
# define RELAYED_CLIENT_HPP

class IRCServer;
#include "RelayedEntity.hpp"
#include "ClientInfo.hpp"

class RelayedClient : public RelayedEntity, public ClientInfo
{
	public:
		static const uint	value_type = RELAYED_CLIENT_ENTITY_TYPE;
    
    public:
		RelayedClient(
			IRCServer		  &server_reference,
			Server            &relay,
			const int         hopcount, //TODO pass it uint
			const std::string &nick,
			const std::string &name,
			const std::string &real_name,
			const uint        mode,
			const std::string &server_token,
			const std::string &server_host
		);

		const std::string		getIdent( void ) const;
		const std::string		getPrefix( void ) const;

};

#endif // RELAYED_CLIENT_HPP