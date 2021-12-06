#include "CommandQuit.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandQuit::CommandQuit(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandQuit::~CommandQuit()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
	Command: QUIT
	Parameters: [ <Quit Message> ]
*/
uint					CommandQuit::operator()(NetworkEntity & executor, std::string params)
{
	std::string quitMessage = Parser::getParam(params, 0);
	std::string prefix = "";
	NetworkEntity* except = NULL;
	if (executor.getFamily() == SERVER_ENTITY_FAMILY)
	{
		//Quit from SERVER
		Logger::debug("Quit relayed connection");
		AEntity& emitter = this->getEmitter();
		if (quitMessage.empty())
			quitMessage = emitter.getUID();
		prefix = emitter.getPrefix();
		except = &executor;
		this->getServer()._entities.erase(emitter.getUID());
		this->getServer()._clients.erase(emitter.getUID());
		Logger::info(emitter.getUID() + " is quitting");
		//Forward this information
		this->getServer()._sendAllServers(prefix + " QUIT " + quitMessage, except);
		this->getServer()._entities.erase(emitter.getUID());
		this->getServer()._clients.erase(emitter.getUID());
		delete &emitter;
		return SUCCESS;
	}
	else
	{
		// Quit FROM direct connection
		Logger::debug("Quit direct connection");
		if (quitMessage.empty())
			quitMessage = executor.getUID();
		prefix = executor.getPrefix();
		Logger::info(executor.getUID() + " is quitting");
		executor.setCleanDisconnection(true);
		this->getServer().disconnect(executor.getStream());
		//Forward this information
		this->getServer()._sendAllServers(prefix + " QUIT " + quitMessage, except);
	}
	return SUCCESS;
}


bool				CommandQuit::hasPermissions(AEntity & executor)
{
	( void) executor;
	return true;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */