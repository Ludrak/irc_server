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
		static const uint	value_type = CLIENT_ENTITY_TYPE;
	
	public:
		Client(
			const IRCServer                 &server_reference,
			const UnRegisteredConnection    &client,
			const uint                      mode,
			const std::string               &real_name
		);
		~Client();
};

#endif /* ********************************************************** CLIENT_H */