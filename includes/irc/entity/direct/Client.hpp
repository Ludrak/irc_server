#ifndef CLIENT_HPP
# define CLIENT_HPP

class IRCServer;
# include "SockStream.hpp"
# include "AEntity.hpp"
# include "IRCServer.hpp"
# include "NetworkEntity.hpp"
# include "UnRegisteredConnection.hpp"

// need ClientInfo + Channel
# include "ClientInfo.hpp"
# include "Channel.hpp"

class Client : public NetworkEntity, public ClientInfo
{
	public:
		static const uint	value_type = CLIENT_ENTITY_TYPE;
	
	public:
		Client(
			IRCServer                 		&server_reference,
			UnRegisteredConnection    		&client,
			const uint                      mode,
			const std::string               &real_name
		);

		const std::string			getIdent( void ) const;
		const std::string			getPrefix( void ) const;

		~Client();
};

#endif /* ********************************************************** CLIENT_H */