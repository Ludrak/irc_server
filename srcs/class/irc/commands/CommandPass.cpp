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

//TODO see ERR_461
uint					CommandPass::operator()(NetworkEntity & executor, std::string params)
{
	if (this->getServer()._entities.count(executor.getUID()) != 0)
	{
		Logger::warning("Alredy registered");
		this->getServer()._sendMessage(executor, ERR_ALREADYREGISTRED(executor.getUID()));
		return SUCCESS;
	}
	UnRegisteredConnection & ue = static_cast<UnRegisteredConnection &>(executor);
	//REVIEW check params syntax
	switch (Parser::nbParam(params))
	{
		case 0:
			Logger::warning("need more params");
			this->getServer()._sendMessage(ue, ERR_NEEDMOREPARAMS(executor.getUID(), std::string("PASS")));
			break ;
		case 3:
			ue.setVersion(Parser::getParam(params, 1));
			ue.setFlags(Parser::getParam(params, 2));
		case 1:
			ue.setPassword(Parser::getParam(params, 0));
			Logger::warning("password set");
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