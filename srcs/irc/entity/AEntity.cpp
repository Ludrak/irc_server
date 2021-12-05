#include "AEntity.hpp"


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AEntity::AEntity(const int type, const std::string &uid) 
:	CommonInfo(uid),
	_type(type | AEntity::value_type),
	_family(ENTITY_FAMILY),
	_creationTime(time(NULL))
{
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

time_t				AEntity::getCreationTime() const
{
	return (this->_creationTime);
}