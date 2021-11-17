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

/*
   Command: PASS
   Parameters: <password> <version> <flags> [<options>]
*/

uint					CommandPass::operator()(NetworkEntity & executor, std::string params)
{
	if (this->getServer()._entities.count(executor.getUID()) != 0)
		return this->getServer()._sendMessage(executor, ERR_ALREADYREGISTRED("PASS"));
	switch (Parser::nbParam(params))
	{
		case 0:
			return this->getServer()._sendMessage(executor, ERR_NEEDMOREPARAMS("PASS"));
		case 3:
			executor.setVersion(Parser::getParam(params, 1));
			executor.setFlags(Parser::getParam(params, 2));
		case 1:
			executor.setPassword(Parser::getParam(params, 0));
			break ;
		default:
			Logger::error("Pass: Bad number of arguments");
			break ;
	}
	return SUCCESS;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandPass::hasPermissions(AEntity & executor)
{
	(void) executor;
	return true;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */