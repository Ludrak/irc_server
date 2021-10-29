#ifndef ADMIN_HPP
# define ADMIN_HPP

# include "Client.hpp"

class Admin : public Clients
{

	public:

		Admin();
		Admin( Admin const & src );
		~Admin();

		Admin &		operator=( Admin const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, Admin const & i );

#endif /* *********************************************************** ADMIN_H */