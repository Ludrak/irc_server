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
   Command: PART
   Parameters: <channel> *( "," <channel> ) [ <Part Message> ]
*/

uint					CommandPart::operator()(NetworkEntity & executor, std::string params)
{
	(void)executor;
	int nParam = Parser::nbParam(params);
	if (nParam == 0)
		this->getServer()._sendMessage(this->getEmitter(), ERR_NEEDMOREPARAMS(this->getEmitter().getPrefix(), "PART"));
	else if (nParam > 0 && nParam < 3)
	{
		std::list<std::string> channels = Parser::paramToList(Parser::getParam(params, 0));
		for (std::list<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			if (executor.getFamily() == CLIENT_ENTITY_FAMILY)
			{
				if (this->getServer()._channels.count(*it) == 0)
				{
					Logger::debug ("Channel not found: " + *it);
					this->getServer()._sendMessage(this->getEmitter(), ERR_NOSUCHCHANNEL(this->getEmitter().getUID(), *it));
					continue;
				}
				else if (!this->getServer()._channels[*it]->isRegistered(this->getEmitter()))
				{
					Logger::debug ("Client not on channel: " + this->getEmitter().getUID());
					this->getServer()._sendMessage(this->getEmitter(), ERR_NOTONCHANNEL(this->getEmitter().getUID(), *it));
					continue;
				}
			}
			Logger::debug("Client leaving channel");
			if (nParam == 1)
			{
				if (this->getEmitter().getType() & Client::value_type)
				{
					this->getServer()._sendMessage(*this->getServer()._channels[*it], this->getEmitter().getPrefix() + "PART " + params);
					static_cast<Client &>(this->getEmitter()).leaveChannel(*this->getServer()._channels[*it]);
				}
				else if (this->getEmitter().getType() & RelayedClient::value_type)
					static_cast<RelayedClient &>(this->getEmitter()).leaveChannel(*this->getServer()._channels[*it]);
			}
			else
			{
				if (this->getEmitter().getType() & Client::value_type)
				{
					static_cast<Client &>(this->getEmitter()).leaveChannel(*this->getServer()._channels[*it], Parser::getParam(params, 1));
				}
				else if (this->getEmitter().getType() & RelayedClient::value_type)
				{
					static_cast<RelayedClient &>(this->getEmitter()).leaveChannel(*this->getServer()._channels[*it], Parser::getParam(params, 1));
				}
			}
			this->getServer()._sendAllServers(this->getEmitter().getPrefix() + "PART " + params, &executor);
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