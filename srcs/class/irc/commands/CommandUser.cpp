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
		this->getServer()._sendMessage(executor, ERR_NEEDMOREPARAMS(executor.getUID(), std::string("USER")));
		return SUCCESS;
	}
	else if (executor.getUID().empty() == true)
		return SUCCESS;
	std::string username = Parser::getParam(params, 0);
	//REVIEW set more params syntax check
	if (Parser::validUser(username) == false)
		return SUCCESS;
	try {
		Client *new_client = new Client(this->getServer(),
									executor,
									std::stoi(Parser::getParam(params, 1)),
									Parser::getParam(params, 3));
		new_client->setName(username);
		this->getServer()._addClient(*new_client, &executor);
		Logger::info("new user registered: " + new_client->getUID());
		/* Send registration informations to new user*/
		this->_sendWelcomeInfos(*new_client);
		//Forward and backward this information:
		std::stringstream reply_msg;
		reply_msg << "NICK " << new_client->getUID() << " 1 " << new_client->getName() << " " << new_client->getHostname() << " " << new_client->getServerToken() << " " << new_client->getModeString() << " " << new_client->getRealname();
		this->getServer()._sendAllServers(reply_msg.str());
	} catch (const std::invalid_argument & e)
	{
		Logger::debug("Invalid mode argument (not a number)");
	}
	return SUCCESS;
}

void		CommandUser::_sendWelcomeInfos(Client & new_client)
{
	std::string pref = new_client.getUID() + "!" + new_client.getName() + "@" + this->getServer().getUID();
	Logger::debug("pref = " + pref);
	//TODO make a IRCServer._reply for adding prefix
	this->getServer()._sendMessage(new_client, ":" + this->getServer().getUID() + " " + RPL_WELCOME(new_client.getUID(), pref));
	this->getServer()._sendMessage(new_client, ":" + this->getServer().getUID() + " " + RPL_YOURHOST(new_client.getUID(), this->getServer().getUID(), this->getServer().getVersion()));
	this->getServer()._sendMessage(new_client, ":" + this->getServer().getUID() + " " + RPL_CREATED(new_client.getUID(), this->getServer().getCreationDate()));
	// this->getServer()._sendMessage(executor, RPL_MYINFO());

}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */