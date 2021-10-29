#include "AEntitie.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AEntitie::AEntitie()
{
}

AEntitie::AEntitie( const AEntitie & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AEntitie::~AEntitie()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

AEntitie &				AEntitie::operator=( AEntitie const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, AEntitie const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */