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
			/* right numbers of params */
			std::string targetName = Parser::getParam(params, 0);
			Logger::debug("Privmsg target: " + targetName); 
			if (this->getServer()._entities.count(targetName) == 0)
			{
				/* target doesn't exist */
				Logger::debug("Privmsg: Target not found");
					this->getServer()._sendMessage(executor, ERR_NOSUCHNICK(executor.getUID(), targetName));
				return SUCCESS;
			}
			AEntity *target = this->getServer()._entities[targetName];
			if (target->getType() & Channel::value_type)
			{
				/* additional tests when target is a channel */
				Logger::debug("Privmsg: target is a channel");
				if (reinterpret_cast<Channel *>(target)->isRegistered(static_cast<Client&>(executor)) == false)
				{
					Logger::debug("Privmsg: sender have not joined channel");
					this->getServer()._sendMessage(executor.getStream(), ERR_CANNOTSENDTOCHAN(executor.getUID(), target->getUID()));
					return SUCCESS;
				}
			}
			std::string msg = Parser::getParam(params, 1);
			// msg = this->getServer().makePrefix(&executor, &this->getServer()) + "PRIVMSG " + target->getUID() + " :" + msg;
			const AEntity *emitter = this->getClient();
			if (emitter == NULL)
			{
				/* No prefix => message from a local client */
				emitter = &executor;
				Logger::info(executor.getUID() + " send message to " + target->getUID());
			}
			else {
				Logger::info(executor.getUID() + " relay a message from " + emitter->getUID() + " to " + target->getUID());
			}
			//add prefix
			msg = emitter->getPrefix() + " PRIVMSG " + target->getUID() + " :" + msg;
			Logger::debug(std::string("MSG = ") + msg);
			this->getServer()._sendMessage(*target, msg, &executor);
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
