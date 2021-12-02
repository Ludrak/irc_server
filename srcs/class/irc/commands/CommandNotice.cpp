#include "CommandNotice.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandNotice::CommandNotice(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandNotice::~CommandNotice()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
	Command: PRIVMSG
	Parameters: <msgtarget> <text to be sent>
*/
uint					CommandNotice::operator()(NetworkEntity & executor, std::string params)
{
	if (Parser::nbParam(params) < 2)
	{
		Logger::warning("Notice: not enought parameters");
		return SUCCESS;
	}
	/* right numbers of params */
	std::string targetName = Parser::getParam(params, 0);
	std::string data = Parser::getParam(params, 1);
	if (data.find('\x01') == std::string::npos)
		Logger::debug("Notice: not a CTCP request");
	else
		Logger::info("Notice: CTCP request");
	Logger::debug("Notice target: " + targetName); 
	if (this->getServer()._entities.count(targetName) == 0)
	{
		/* target doesn't exist */
		Logger::debug("Notice: Target not found !");
		return SUCCESS;
	}
	AEntity *target = this->getServer()._entities[targetName];
	if (target->getType() & Channel::value_type)
	{
		/* additional tests when target is a channel */
		Logger::debug("Notice: target is a channel");
		if (reinterpret_cast<Channel *>(target)->isRegistered(static_cast<Client&>(executor)) == false)
		{
			Logger::debug("Notice: not in channel !");
			return SUCCESS;
		}
	}
	AEntity& emitter = this->getEmitter();
	//REVIEW replace `replay` by `send` here and in PrivmsgCommand
	Logger::info(executor.getUID() + " relay a notice from " + emitter.getUID() + " to " + target->getUID());
	std::string msg = emitter.getPrefix() + "NOTICE " + target->getUID() + " :" + Parser::getParam(params, 1);
	Logger::debug("Notice = " + msg); //REVIEW can be removed because already printed in sendMessage
	this->getServer()._sendMessage(*target, msg);
	return SUCCESS;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandNotice::hasPermissions(AEntity & executor)
{
	if (executor.getType() & UnRegisteredConnection::value_type)
		return false;
	return true;
}
