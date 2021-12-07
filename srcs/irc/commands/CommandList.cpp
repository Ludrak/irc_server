#include "CommandList.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandList::CommandList(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandList::~CommandList()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
Command: LIST
   Parameters: [ <channel> *( "," <channel> ) [ <target> ] ]
*/
uint					CommandList::operator()(NetworkEntity & executor, std::string params)
{
	(void) executor;
	AEntity &emitter = this->getEmitter();
	uint nbParam = Parser::nbParam(params);
	Logger::debug("List: with " + ntos(nbParam) + " params");
	if ( nbParam == 2)
	{
		/* Forward request before evaluation */
		std::string serverName = Parser::getParam(params, 1);
		if (serverName != this->getServer().getUID())
		{
			if (this->getServer()._servers.count(serverName) == 0)
			{
				this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + ERR_NOSUCHSERVER(emitter.getUID(), serverName));
				return SUCCESS;
			}
			AEntity *serv = this->getServer()._servers[serverName];
			this->getServer()._sendMessage(*serv, emitter.getPrefix() + "LIST " + params);
			return SUCCESS;
		}
	}
	else if (nbParam == 1)
	{
		/* Return a result only for listed channels */
		std::list<std::string> chanList = Parser::paramToList(Parser::getParam(params, 0));
		for (std::list<std::string>::iterator it = chanList.begin(); it != chanList.end(); ++it)
		{
			Logger::debug("List: testing channel: " + *it);
			/* Check if it's an existing channel name */
			if (this->getServer()._channels.count(*it) == 1)
			{
				Channel *chan = this->getServer()._channels[*it];
				this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + RPL_LIST(emitter.getUID(), chan->getUID(), ntos(chan->getConcurrentClients()), chan->getTopic()));
			}
		}
		this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + RPL_LISTEND(emitter.getUID()));
		return SUCCESS;
	}
	Logger::debug("List: sending full channels list");
	/* Return the list of available channels */
	for (std::map<std::string, Channel*>::iterator it = this->getServer()._channels.begin(); it != this->getServer()._channels.end(); ++it)
	{
		this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + RPL_LIST(emitter.getUID(), it->second->getUID(), ntos(it->second->getConcurrentClients()) , it->second->getTopic()));
	}
	this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + RPL_LISTEND(emitter.getUID()));
	return SUCCESS;
}


bool				CommandList::hasPermissions(AEntity & executor)
{
	/* Usable only by registered users */
	if (executor.getType() & UnRegisteredConnection::value_type)
		return false;
	return true;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */