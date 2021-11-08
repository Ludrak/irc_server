#include "AEntity.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AEntity::AEntity()
{
}

AEntity::AEntity( const AEntity & src )
{
	(void) src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AEntity::~AEntity()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// AEntity &				AEntity::operator=( AEntity const & rhs )
// {
// 	//if ( this != &rhs )
// 	//{
// 		//this->_value = rhs.getValue();
// 	//}
// 	return *this;
// }

// std::ostream &			operator<<( std::ostream & o, AEntity const & i )
// {
// 	//o << "Value = " << i.getValue();
// 	return o;
// }


/*
** --------------------------------- METHODS ----------------------------------
*/

uint						AEntity::getUID( void ) const
{
	return this->_uid;
}


std::string					AEntity::getNickname(void)
{
	return this->_nickname;
}

void						AEntity::setNickname(std::string nick)
{
	this->_nickname = nick;
}

std::string					AEntity::getPassword( void )
{
	return this->_givenPassword;
}

void						AEntity::setPassword( std::string password) 
{
	this->_givenPassword = password;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */