#include "ASockstream.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ASockstream::ASockstream()
{
}

ASockstream::ASockstream( const ASockstream & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ASockstream::~ASockstream()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ASockstream &				ASockstream::operator=( ASockstream const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, ASockstream const & i )
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