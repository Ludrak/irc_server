#include "CommandPass.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandPass::CommandPass() : ACommand()
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandPass::~CommandPass()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

uint					CommandPass::operator()(AEntity & executor, std::string params)
{
	(void) executor;
	(void) params;
	return SUCCESS;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandPass::hasPermissions(AEntity & executor)
{
	//TODO implement right for NICK
	( void) executor;
	return SUCCESS;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */