#include "CommandOper.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandOper::CommandOper(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandOper::~CommandOper()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

uint					CommandOper::operator()(NetworkEntity & executor, std::string params)
{
	//TODO add list of operName
	//TODO more globaly: add config file and parse it 
	if (Parser::nbParam(params) < 2)
	{
		this->getServer()._sendMessage(executor, this->getServer().makePrefix(NULL, NULL) + ERR_NEEDMOREPARAMS(executor.getUID(), std::string("OPER")));
		return SUCCESS;
	}
	else if (executor.getType() & Client::value_type)
	{
		std::string name = Parser::getParam(params, 0);
		std::string password = Parser::getParam(params, 1);
		if (this->getServer()._operName != name)
		{
			Logger::debug("Invalid oper name");
			return SUCCESS;
		}
		else if (this->getServer()._operPassword != password)
		{
			this->getServer()._sendMessage(executor, this->getServer().makePrefix(NULL, NULL) + ERR_PASSWDMISMATCH(executor.getUID()));
			return SUCCESS;
		}
		//Here all is right: becoming a IRC operator
		static_cast<Client&>(executor).setServerOP(true);
		Logger::info(executor.getUID() + " become a server operator.");
		this->getServer()._sendMessage(executor, this->getServer().makePrefix(NULL, NULL) + RPL_YOUREOPER(executor.getUID()));
		//TODO send mode +o message to other servers
		this->getHandler().handle(executor, ":" + executor.getUID() + "@" + static_cast<Client&>(executor).getServerToken() + " MOTD");
	}
	else
	{
		Logger::critical("Oper: unhandled OPER command from Server");
	}
	return SUCCESS;
}

bool				CommandOper::hasPermissions(AEntity & executor)
{
	//TODO implement right for OPER
	if (executor.getType() & UnRegisteredConnection::value_type)
		return false;
	return true;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */