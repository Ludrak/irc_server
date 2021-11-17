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
		this->getServer()._sendMessage(executor, ERR_ALREADYREGISTRED());
	else if (executor.getType() & UnRegisteredConnection::value_type)
		return this->_commandUSERunknown(static_cast<UnRegisteredConnection&>(executor), params);
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

uint				CommandUser::_commandUSERunknown(UnRegisteredConnection & executor, std::string params)
{
	if (Parser::nbParam(params) < 4)
	{
		this->getServer()._sendMessage(executor, ERR_NEEDMOREPARAMS(std::string("USER")));
		return SUCCESS;
	}
	else if (executor.getUID().empty() == true)
		return SUCCESS;
	std::string username = Parser::getParam(params, 0);
	//REVIEW set more params syntax check
	if (Parser::validUser(username) == false)
		return SUCCESS;
	//REVIEW catch throw in stoi
	Client *new_client = new Client(this->getServer(),
								executor,
								std::stoi(Parser::getParam(params, 1)),
								Parser::getParam(params, 3));
	new_client->setName(username);
	this->getServer()._addClient(*new_client, &executor);
	Logger::info("new user registered: " + new_client->getUID());
	return SUCCESS;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */