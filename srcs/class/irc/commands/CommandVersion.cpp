#include "CommandVersion.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandVersion::CommandVersion(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandVersion::~CommandVersion()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
	Command: VERSION
	Parameters: [ <target> ]
*/
uint					CommandVersion::operator()(NetworkEntity & executor, std::string params)
{
	IRCServer&	master = this->getServer();
	AEntity&	emitter = this->getEmitter();

	if (Parser::nbParam(params) == 0)
		return this->_sendVersion(master, emitter);
	
	/* Target server given */
	std::string serverId = Parser::getParam(params, 0);
	/* Check if target is current server */
	if (master.getUID() == serverId)
		return this->_sendVersion(master, emitter);
	else if (master._servers.count(serverId) == 0)
	{
		/* Check if target exist */
		master._sendMessage(executor, emitter.getPrefix() + ERR_NOSUCHSERVER(emitter.getUID(), serverId));
		return SUCCESS;
	}
	/* Redirect to corresponding server */
	AEntity *forward = master._servers[serverId];
	if (forward->getType() & NetworkEntity::value_type)
		master._sendMessage(*forward, emitter.getPrefix() + "VERSION");
	else
		master._sendMessage(*forward, emitter.getPrefix() + "VERSION " + serverId);
	return SUCCESS;
}

uint				CommandVersion::_sendVersion(IRCServer &master, AEntity & emitter)
{
	this->getServer()._sendMessage(emitter,
		RPL_VERSION(
			emitter.getUID(),
			master.getVersion(),
			ntos(master.getDebugLevel()),
			master.getUID(),
			std::string("Any other intresting info about the version")
		)
	);
	return SUCCESS;
}

bool				CommandVersion::hasPermissions(AEntity & executor)
{
	if (executor.getType() & UnRegisteredConnection::value_type)
		return false;
	return true;
}

/* ************************************************************************** */