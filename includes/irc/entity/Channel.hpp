#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "AEntity.hpp"
# include "Client.hpp"
# include "ASockManager.hpp"

class Channel : public AEntity, public ASockHandler
{

	public:

		Channel();
		Channel( Channel const & src );
		~Channel();

		Channel &		operator=( Channel const & rhs );
		
		enum C { value_type = 0 };

		uint				getType( void ) const;

		void				broadcastPackage(Package * pkg, const SockStream * except = NULL);

	private:
		std::list<Client *> 	_clients;
		std::list<Client *> 	_operators;
};

std::ostream &			operator<<( std::ostream & o, Channel const & i );

#endif /* ********************************************************* CHANNEL_H */