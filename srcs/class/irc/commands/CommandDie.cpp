#include "CommandDie.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandDie::CommandDie(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandDie::~CommandDie()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/
//TODO sendPackage is not _sendPackage

/*
	Command: DIE
	Parameters: None
*/
uint					CommandDie::operator()(NetworkEntity & executor, std::string params)
{
	(void) params;
	Client& emitter = static_cast<Client&>(executor);
	if (emitter.isServerOP() == false)
	{
		this->getServer()._sendMessage(executor, this->getServer().getPrefix() + ERR_NOPRIVILEGES(executor.getUID()));
		return SUCCESS;
	}
	Logger::warning("Die: OP " + emitter.getIdent() + " shutdown the server.");
	/* disconnect all connections */
	this->_kickAllConnections();
	/* Shutdown the server */
	this->getServer().shutdown();
	return SUCCESS;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

//TODO put this methods (or very similar equivalent) in SockManager
uint				CommandDie::_kickAllConnections( void)
{
	for (std::map<SockStream*, NetworkEntity*>::iterator it = this->getServer()._connections.begin();
		it != this->getServer()._connections.end();)
	{
		Logger::info("manually disconnecting: " + it->second->getUID());
		this->getServer().disconnect((it++)->second->getStream());
	}
	return SUCCESS;
}

bool				CommandDie::hasPermissions(AEntity & executor)
{
	// if (executor.getType() & Client::value_type)
		// return true;
	if (executor.getType() & Client::value_type)
		return true;
	return false;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */