#include "Package.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Package::Package()
{
}

Package::Package( const Package & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Package::~Package()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Package &				Package::operator=( Package const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Package const & i )
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