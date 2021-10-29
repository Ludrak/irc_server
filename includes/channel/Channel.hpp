#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include "AEntitie.hpp"
# include "Client.hpp"

class Channel : public AEntities
{

	public:

		Channel();
		Channel( Channel const & src );
		~Channel();

		Channel &		operator=( Channel const & rhs );

	private:
		std::list<Clients &> 	_clients;
};

std::ostream &			operator<<( std::ostream & o, Channel const & i );

#endif /* ********************************************************* CHANNEL_H */