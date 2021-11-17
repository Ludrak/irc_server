#include "CommandUser.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandUser::CommandUser(CommandHandler & handler) : ACommand(handler)
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

uint					CommandUser::operator()(NetworkEntity & executor, std::string params)
{
	( void ) executor;
	( void ) params;
	return SUCCESS;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandUser::hasPermissions(AEntity & executor)
{
	//TODO implement right for NICK
	( void) executor;
	return true;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */