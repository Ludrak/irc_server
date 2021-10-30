#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "AEntity.hpp"
# include "Client.hpp"

class Channel : public AEntity
{

	public:

		Channel();
		Channel( Channel const & src );
		~Channel();

		Channel &		operator=( Channel const & rhs );

	private:
		std::list<Client *> 	_clients;
		std::list<Client *> 	_operators;
};

std::ostream &			operator<<( std::ostream & o, Channel const & i );

#endif /* ********************************************************* CHANNEL_H */