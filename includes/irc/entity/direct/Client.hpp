#ifndef CLIENT_HPP
# define CLIENT_HPP

class IRCServer;
# include "SockStream.hpp"
# include "AEntity.hpp"
# include "IRCServer.hpp"
# include "NetworkEntity.hpp"
# include "ClientInfo.hpp"
# include "UnRegisteredConnection.hpp"

class Client : public NetworkEntity, public ClientInfo
{
	public:
		static const uint	value_type;
	
	public:
		Client (const UnRegisteredConnection &client);
};

const uint Client::value_type = CLIENT_ENTITY_TYPE;

#endif /* ********************************************************** CLIENT_H */