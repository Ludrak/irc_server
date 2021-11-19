#include "CommandNjoin.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandNjoin::CommandNjoin(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandNjoin::~CommandNjoin()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

uint					CommandNjoin::operator()(NetworkEntity & executor, std::string params)
{
    size_t nbParam = Parser::nbParam(params);
	if ( nbParam == 0)
	{
        this->getServer()._sendMessage(executor, ERR_NEEDMOREPARAMS(std::string("JOIN")));
        return SUCCESS;
    }
	std::string channels((Parser::getParam(params, 0)));

    std::list<std::string> member(Parser::paramToList(Parser::getParam(params, 1)));

	for (std::list<std::string>::iterator itMember = channels.begin(); itMember != channels.end(); ++itMember)
	{
		
	}
	return SUCCESS;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandNjoin::hasPermissions(AEntity & executor)
{
	if (executor.getFamily() == CLIENT_ENTITY_FAMILY)
	    return true;
    return (false);
}

/* ************************************************************************** */