#ifndef AENTITIE_HPP
# define AENTITIE_HPP

# include <iostream>
# include <string>
# include <list>

class AEntitie
{

	public:

		AEntitie();
		AEntitie( AEntitie const & src );
		~AEntitie();

		AEntitie &		operator=( AEntitie const & rhs );

		uint		getId( void );
	private:
		uint		id;

};

std::ostream &			operator<<( std::ostream & o, AEntitie const & i );

#endif /* ******************************************************** AENTITIE_H */