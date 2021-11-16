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

uint				AEntity::getType() const
{
	return (this->_type);
}

const std::string&	AEntity::getUID( void ) const
{
	return this->_uid;
};
