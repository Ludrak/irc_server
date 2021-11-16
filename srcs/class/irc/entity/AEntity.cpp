#include "AEntity.hpp"


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AEntity::AEntity(const int type, const std::string &uid) : _type(type | AEntity::value_type), _uid(uid), _family(ENTITY_FAMILY)
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

const std::string&	AEntity::getUID( void ) const
{
	return this->_uid;
}

void				AEntity::setUID( const std::string & uid )
{
	this->_uid = uid;
}
