#include "CommandPong.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandPong::CommandPong(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandPong::~CommandPong()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/
//TODO sendPackage is not _sendPackage

/*
	Command: PONG
	parameters: <server> [ <server2> ]
   NOT IMPLEMENTED
*/
uint					CommandPong::operator()(NetworkEntity & executor, std::string params)
{
	(void) executor;
	(void) params;
	return SUCCESS;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandPong::hasPermissions(AEntity & executor)
{
	// if (executor.getType() & Client::value_type)
		// return true;
	(void) executor;
	return true;
}


/* ************************************************************************** */