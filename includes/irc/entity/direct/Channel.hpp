#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "AEntity.hpp"
# include "Client.hpp"
# include "ASockManager.hpp"
# include "ChannelInfo.hpp"

# define NB_CHANNEL_REGISTRATION_MAX 12

# define M_MODERATED		1 << 1

class Channel : public AEntity, public ASockHandler, public ChannelInfo
{
	public:
		static const uint	value_type = CHANNEL_ENTITY_TYPE;
	
	public:
		Channel(const std::string & channelName);
		Channel( Channel const & src );
		~Channel();

		Channel &		operator=( Channel const & rhs );

		Client				*getCreator(void);
		void				setCreator(Client & client);

		uint				addClient(Client & client);
		uint				removeClient(Client & client);
		bool				isRegistered(Client & client);
		void				broadcastPackage(Package & pkg, const SockStream * except = NULL);
		void            	delSocket(const SockStream &sock);

		//TODO := check for passing M_REGISTRATION_LIMITED to AEntity beacause used by Client too and add a test on this flag before sayin 'cannot inc or dec'
	private:
		Client*					_creator; //REVIEW Check where to use it
		std::list<Client *> 	_clients;
		std::list<Client *> 	_operators;
};

#endif /* ********************************************************* CHANNEL_H */