#include "CommandPart.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandPart::CommandPart(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandPart::~CommandPart()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
   Command: PASS
   Parameters: <password> <version> <flags> [<options>]
*/

uint					CommandPart::operator()(NetworkEntity & executor, std::string params)
{
	//Parser::paramToList(params);
	int nParam = Parser::nbParam(params);
	if (nParam == 0)
		this->getServer()._sendMessage(executor, ERR_NEEDMOREPARAMS(executor.getPrefix(), "PART"));
	//TODO Handle server-server
	else if (nParam > 0 && nParam < 3)
	{
		std::list<std::string> channels = Parser::paramToList(Parser::getParam(params, 0));
		for (std::list<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			if (this->getServer()._channels.count(*it) == 0)
			{
				this->getServer()._sendMessage(executor, ERR_NOSUCHCHANNEL(executor.getUID(), *it));
				continue;
			}
			else if (!this->getServer()._channels[*it]->isRegistered(static_cast<Client&>(executor)))
			{
				this->getServer()._sendMessage(executor, ERR_NOTONCHANNEL(executor.getUID(), *it));
				continue;
			}
			if (nParam == 1)
				static_cast<Client &>(executor).leaveChannel(*this->getServer()._channels[*it]);
			else
				static_cast<Client &>(executor).leaveChannel(*this->getServer()._channels[*it], Parser::getParam(params, 1));
		}
	}
	return SUCCESS;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandPart::hasPermissions(AEntity & executor)
{
	if (executor.getType() & UNREGISTERED_CONNECTION_ENTITY_TYPE)
		return (false);
	return true;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */