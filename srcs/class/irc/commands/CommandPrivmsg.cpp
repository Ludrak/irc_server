#include "CommandPrivmsg.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandPrivmsg::CommandPrivmsg(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandPrivmsg::~CommandPrivmsg()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

uint					CommandPrivmsg::operator()(NetworkEntity & executor, std::string params)
{
	switch (Parser::nbParam(params))
	{
		case 0:
			this->getServer()._sendMessage(executor, ERR_NORECIPIENT(params));
			break ;
		case 1:
			this->getServer()._sendMessage(executor, ERR_NOTEXTTOSEND());
			break ;
		case 2:
			//* right numbers of params */
			std::string targetName = Parser::getParam(params, 0);
			Logger::debug("Privmsg target: " + targetName); 
			if (this->getServer()._entities.count(targetName) == 0)
			{
				Logger::debug("Privmsg: Target not found"); 
				this->getServer()._sendMessage(executor, ERR_NOSUCHNICK(targetName));
				return SUCCESS;
			}
			AEntity *target = this->getServer()._entities[targetName];
			if (target->getType() & Channel::value_type)
			{
				Logger::debug("Privmsg: target is a channel");
				if (reinterpret_cast<Channel *>(target)->isRegistered(static_cast<Client&>(executor)) == false)
				{
					Logger::debug("Privmsg: not in channel");
					this->getServer()._sendMessage(executor.getStream(), ERR_CANNOTSENDTOCHAN(target->getUID()));
					return SUCCESS;
				}
			}
			Logger::info(executor.getUID() + " send message to " + target->getUID());
			std::string msg = Parser::getParam(params, 1);
			// msg = this->getServer().makePrefix(&executor, &this->getServer()) + "PRIVMSG " + target->getUID() + " :" + msg;
			if (this->getClient() != NULL)
				msg = this->getServer().makePrefix(this->getClient(), &this->getServer()) + "PRIVMSG " + target->getUID() + " :" + msg;
			else
				msg = this->getServer().makePrefix(&executor, &this->getServer()) + "PRIVMSG " + target->getUID() + " :" + msg;
			Logger::debug(std::string("MSG = ") + msg);
			this->getServer()._sendMessage(*target, msg);
			break ;
	}
	return SUCCESS;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandPrivmsg::hasPermissions(AEntity & executor)
{
	if (executor.getType() & UnRegisteredConnection::value_type)
		return false;
	return true;
}
