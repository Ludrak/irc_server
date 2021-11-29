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
			IRCServer                 		&server_reference,
			const UnRegisteredConnection    &client,
			const uint                      mode,
			const std::string               &real_name
		);

		const std::list<Channel*>	&getChannels() const;
		int							joinChannel(Channel &channel);
		void						leaveChannel(Channel &channel, const std::string &info="");
		void                        leaveAllChannels(const std::string &info="");
		const std::string			getIdent( void ) const;
		const std::string			getPrefix( void ) const;

		~Client();
	
	private:
		std::list<Channel*>		_channels;
};

#endif /* ********************************************************** CLIENT_H */