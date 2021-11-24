#include "CommandError.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandError::CommandError(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandError::~CommandError()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
   Command: ERROR
   Parameters: <error message>
*/

uint					CommandError::operator()(NetworkEntity & executor, std::string params)
{
	(void)executor;
	// TODO send all to SQUIT executor
	if (!params.empty())
		Logger::warning("RECEIVED ERROR FROM SERVER: " + params.substr(1, params.size() - 1));
	else
		Logger::warning("RECEIVED ERROR FROM SERVER");
	return (SUCCESS);
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandError::hasPermissions(AEntity & executor)
{
	if (executor.getType() & UnRegisteredConnection::value_type)
		return (false);
	return true;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */