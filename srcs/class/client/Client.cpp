#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client() : ASockStream(), AEntity()
{
}

Client::Client(ASockStream &master) :  ASockStream(master), AEntity()
{
	
}

Client::Client( const Client & src ) : ASockStream(src), AEntity(src)
{
	(void) src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client &				Client::operator=( Client const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
		(void) rhs;
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Client const & i )
{
	//o << "Value = " << i.getValue();
	(void) i;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */