#include "AEntity.hpp"


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AEntity::AEntity(const int type, const std::string &uid) : _type(type | AEntity::value_type), _uid(uid)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AEntity::~AEntity()
{
}

int					AEntity::getType() const
{
	return (this->_type);
}

const std::string&	AEntity::getUID( void ) const
{
	return this->_uid;
};


// /*
// ** --------------------------------- METHODS ----------------------------------
// */

// uint						AEntity::getUID( void ) const
// {
// 	return this->_uid;
// }


// std::string					AEntity::getNickname(void)
// {
// 	return this->_nickname;
// }

// void						AEntity::setNickname(std::string nick)
// {
// 	this->_nickname = nick;
// }

// std::string					AEntity::getPassword( void )
// {
// 	return this->_givenPassword;
// }

// void						AEntity::setPassword( std::string password) 
// {
// 	this->_givenPassword = password;
// }


// bool						AEntity::incRegistration( void )
// {
// 	if (this->isEnable(M_REGISTRATION_LIMITED) && this->_nbRegistrations >= this->_nbRegistrationsMax)
// 		return false;
// 	this->_nbRegistrations++;
// 	return true;
// }

// bool						AEntity::decRegistration( void )
// {
// 	if (this->_nbRegistrations == 0)
// 		return false;
// 	this->_nbRegistrations--;
// 	return true;
// }

// uint						AEntity::getNbRegistrations( void ) const
// {
// 	return this->_nbRegistrations;
// }

// uint						AEntity::getNbRegistrationsMax( void ) const
// {
// 	return this->_nbRegistrationsMax;
// }

// bool						AEntity::isFull( void ) const
// {
// 	if (this->_nbRegistrations == this->_nbRegistrationsMax)
// 		return true;
// 	return false;
// }


// void						AEntity::toogleMode(uint modeMask)
// {
// 	this->_mode ^= modeMask;
// }

// void						AEntity::enableMode(uint modeMask)
// {
// 	this->_mode |= modeMask;
// }

// void						AEntity::disableMode(uint modeMask)
// {
// 	this->_mode &= ~modeMask;
// }

// bool						AEntity::isEnable(uint modeMask)
// {
// 	return (this->_mode & modeMask);	
// }

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */