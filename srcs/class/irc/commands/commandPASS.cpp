#include "CommandPass.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandPass::CommandPass(CommandHandler & handler) : ACommand(handler)
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

uint					CommandPass::operator()(NetworkEntity & executor, std::string params)
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
	return true;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */