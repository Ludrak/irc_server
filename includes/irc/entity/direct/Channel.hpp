#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "AEntity.hpp"
# include "ASockManager.hpp"
# include "ChannelInfo.hpp"
# include "NetworkEntity.hpp"
# include "RelayedEntity.hpp"

# define NB_CHANNEL_REGISTRATION_MAX 12

# define M_MODERATED		1 << 1

class Channel : public AEntity, public ChannelInfo
{
	public:
		static const uint	value_type = CHANNEL_ENTITY_TYPE;
	
	public:
		Channel(const std::string & channelName);
		Channel( Channel const & src );
		~Channel();

		Channel &		operator=( Channel const & rhs );

		const AEntity		*getCreator(void) const;
		void				setCreator(const AEntity & client);

		uint				addClient(AEntity & client);
		uint				removeClient(AEntity & client);
		bool				isRegistered(AEntity & client);
		void				broadcastPackage(Package & pkg, const std::string &uid="");
		void            	delSocket(const SockStream &sock);

		const std::string	&getTopic(void) const;
		void				setTopic(const std::string &new_topic);
	
		std::list<AEntity *>::iterator		clientBegin( void );
		std::list<AEntity *>::const_iterator	clientBegin( void ) const;
		std::list<AEntity *>::iterator		clientEnd( void );
		std::list<AEntity *>::const_iterator	clientEnd( void ) const;
		std::list<AEntity *>::iterator		operatorBegin( void );
		std::list<AEntity *>::const_iterator	operatorBegin( void ) const;
		std::list<AEntity *>::iterator		operatorEnd( void );
		std::list<AEntity *>::const_iterator	operatorEnd( void ) const;

		//TODO := check for passing M_REGISTRATION_LIMITED to ClientInfo beacause used by Client too and add a test on this flag before sayin 'cannot inc or dec'
	private:
		const AEntity*			_creator;
		std::list<AEntity *>	_clients;
		std::list<AEntity *>	_operators;
		std::string				_topic;
};

# include "Client.hpp"

#endif /* ********************************************************* CHANNEL_H */