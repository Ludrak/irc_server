#include "Admin.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Admin::Admin()
{
}

Admin::Admin( const Admin & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Admin::~Admin()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Admin &				Admin::operator=( Admin const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Admin const & i )
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