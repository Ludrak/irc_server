#include "AEntity.hpp"


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AEntity::AEntity(const int type, const std::string &uid, const struct timeval *creation)
:	CommonInfo(uid),
	_type(type | AEntity::value_type),
	_family(ENTITY_FAMILY)
{
	if (creation)
		_creationTime = *creation;
	else
		gettimeofday(&_creationTime, NULL);
	
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AEntity::~AEntity()
{
}

uint				AEntity::getType( void ) const
{
	return (this->_type);
}

uint				AEntity::getFamily( void ) const
{
	return (this->_family);
}

const std::string	AEntity::getPrefix( void ) const
{
	return ":" + this->_uid + " ";
}

const struct timeval	&AEntity::getCreationTime( void ) const
{
	return (this->_creationTime);
}