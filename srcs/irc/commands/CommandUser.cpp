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

/*
	Command: USER
	Parameters: <user> <mode> <unused> <realname>
*/
uint					CommandUser::operator()(NetworkEntity & executor, std::string params)
{
	if (this->getServer()._entities.count(executor.getUID()) != 0)
		this->getServer()._sendMessage(executor, ERR_ALREADYREGISTRED(executor.getUID()));
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
	if (executor.getFamily() == SERVER_ENTITY_FAMILY)
		return false;
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
	std::istringstream is(Parser::getParam(params, 1));
	int mode = -1;
	is >> mode;
	if (mode < 0)
	{
		Logger::debug("Invalid mode argument (not a number)");
		return SUCCESS;
	}	
	Client *new_client = new Client(this->getServer(),
								executor,
								mode,
								Parser::getParam(params, 3));
	new_client->setName(username);
	this->getServer()._addClient(*new_client, &executor);
	Logger::info("new client registered: " + new_client->getName() + " (" + new_client->getUID() + "!" + new_client->getName() + "@"
													 + new_client->getHostname() + ")");
	/* Send registration informations to new user */
	this->_sendWelcomeInfos(*new_client);
	//Forward and backward this information:
	std::stringstream reply_msg;
	reply_msg << "NICK " << new_client->getUID() << " 1 " << new_client->getName() <<
		" " << new_client->getHostname() << " " << new_client->getServerToken() <<
		" " << new_client->getModeString() << " :" << new_client->getRealname();
	this->getServer()._sendAllServers(reply_msg.str());
	return SUCCESS;
}

void		CommandUser::_sendWelcomeInfos(Client & new_client)
{
	std::string pref = new_client.getUID() + "!" + new_client.getName() + "@" + this->getServer().getUID();
	Logger::debug("pref = " + pref);
	this->getServer()._sendMessage(new_client, this->getServer().getPrefix() + RPL_WELCOME(new_client.getUID(), pref));
	this->getServer()._sendMessage(new_client, this->getServer().getPrefix() + RPL_YOURHOST(new_client.getUID(), this->getServer().getUID(), this->getServer().getVersion()));
	this->getServer()._sendMessage(new_client, this->getServer().getPrefix() + RPL_CREATED(new_client.getUID(), Logger::timeToString(this->getServer().getCreationTime())));
	// this->getServer()._sendMessage(executor, RPL_MYINFO());
	//TODO do RPL_MYINFO
	std::string cmdName("MOTD");
	CommandMotd* cmdMOTD = reinterpret_cast<CommandMotd*>(this->getHandler().getCommand(cmdName));
	if (!cmdMOTD)
	{
		Logger::error("User: Unkown command Motd.");
		return ;
	}
	cmdMOTD->setEmitter(new_client);
	(*cmdMOTD)(new_client, "", true);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */