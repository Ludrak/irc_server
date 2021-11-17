#include "AEntity.hpp"


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AEntity::AEntity(const int type, const std::string &uid) : CommonInfo(uid), _type(type | AEntity::value_type), _family(ENTITY_FAMILY)
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
