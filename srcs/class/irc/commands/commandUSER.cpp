#include "CommandUser.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandUser::CommandUser(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandUser::~CommandUser()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

uint					CommandUser::operator()(NetworkEntity & executor, std::string params)
{
	if (this->getServer()._entities.count(executor.getUID()) != 0)
		return this->getServer()._sendMessage(executor, ERR_ALREADYREGISTRED());
	else if (executor.getType() & UnregisteredConnection::value_type)
		return _commandUSERunknown(static_cast<UnRegisteredConnection&>(executor));
	else
	{
		Logger::critical("Error: Invalid familly/type: " + ntos(executor.getFamily()) + "/" + ntos(executor.getType()));
		return -1;
	}
	return SUCCESS;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandUser::hasPermissions(AEntity & executor)
{
	//TODO implement right for USER
	// Do we steel receive from SERVER?
	// isn't it only by Client or unregistered client?
	( void) executor;
	return true;
}

uint				CommandUser::_commandUSERunknown(UnRegisteredConnection * executor, std::string params)
{
	if (Parser::nbParam(params) < 4)
		return this->_sendMessage(executor, ERR_NEEDMOREPARAMS("USER"));
	else if (executor.getUID().empty() == true)
		return SUCCESS;
	std::string username = Parser::getParam(params, 0);
	//REVIEW set more params syntax check
	if (Parser::validUser(username) == false)
		return SUCCESS;
	Client *new_client = Client(this->getServer(), 
								executor,
								Parser::getParam(params, 1),
								Parser::getParam(params, 3));
	new_client->setName(username);
	this->_addClient(executor);
	Logger::info("new user registered: " + new_client.getNickname());
	return SUCCESS;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */