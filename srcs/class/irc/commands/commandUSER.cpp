#include "CommandUser.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandUser::CommandUser()
{
}

CommandUser::CommandUser( const CommandUser & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandUser::~CommandUser()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

CommandUser &				CommandUser::operator=( CommandUser const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

uint					CommandUser::operator()(AEntity & executor, std::string params)
{
	( void ) executor;
	( void ) params;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandUser::hasPermissions(AEntity & executor)
{
	//TODO implement right for NICK
	( void) executor;
	return SUCCESS;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */