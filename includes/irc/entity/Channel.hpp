#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "AEntity.hpp"
# include "Client.hpp"
# include "ASockManager.hpp"

# define NB_CHANNEL_REGISTRATION_MAX 12

class Channel : public AEntity, public ASockHandler
{

	public:

		Channel(const std::string & channelName);
		Channel( Channel const & src );
		~Channel();

		Channel &		operator=( Channel const & rhs );
		
		uint				getType( void ) const;

		uint				addClient(Client & client);
		uint				removeClient(Client & client);
		bool				isRegistered(Client & client);
		void				broadcastPackage(Package & pkg, const SockStream * except = NULL);
		void            	delSocket(const SockStream &sock);

	private:
		std::list<Client *> 	_clients;
		std::list<Client *> 	_operators;

};

std::ostream &			operator<<( std::ostream & o, Channel const & i );

#endif /* ********************************************************* CHANNEL_H */