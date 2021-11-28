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
		const AEntity* origin = this->getClient();
		if (origin == NULL)
		{
			Logger::critical("No client references given");
			return SUCCESS;
		}
		if (quitMessage.empty())
			quitMessage = origin->getUID();
		prefix = this->getServer().makePrefix(origin, &this->getServer());
		except = &executor;
		this->getServer()._entities.erase(origin->getUID());
		this->getServer()._clients.erase(origin->getUID());
		Logger::info(origin->getUID() + " is quitting");
		delete origin;
	}
	else
	{
		// Quit FROM direct connection
		Logger::debug("Quit direct connection");
		if (quitMessage.empty())
			quitMessage = executor.getUID();
		prefix = this->getServer().makePrefix(&executor, &this->getServer());
		Logger::info(executor.getUID() + " is quitting");
		this->getServer().disconnect(executor.getStream());
		
	}

	//Forward this information
	std::stringstream ss;
	ss << prefix << " QUIT " << quitMessage;
	this->getServer()._sendAllServers(ss.str(), except);
	return SUCCESS;
}


bool				CommandQuit::hasPermissions(AEntity & executor)
{
	//TODO implement right for QUIT
	( void) executor;
	return true;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */