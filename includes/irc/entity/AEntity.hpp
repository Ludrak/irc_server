#ifndef AENTITY_HPP
# define AENTITY_HPP

# include <iostream>
# include <string>
# include <list>
# include <map>

class AEntity
{

	public:

		AEntity();
		AEntity( AEntity const & src );
		~AEntity();

		AEntity &		operator=( AEntity const & rhs );

		uint					getUID( void ) const;
		virtual uint			getId( void ) const = 0;
	private:
		uint		_uid;

};

std::ostream &			operator<<( std::ostream & o, AEntity const & i );

#endif /* ******************************************************** AENTITY_HPP */