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

/*
	Command: PRIVMSG
	Parameters: <msgtarget> <text to be sent>
*/
uint					CommandPrivmsg::operator()(NetworkEntity & executor, std::string params)
{
	(void)executor;
	switch (Parser::nbParam(params))
	{
		case 0:
			this->getServer()._sendMessage(this->getEmitter(), this->getServer().getPrefix() + ERR_NORECIPIENT(this->getEmitter().getUID(), "PRIVMSG"));
			break ;
		case 1:
			this->getServer()._sendMessage(this->getEmitter(), this->getServer().getPrefix() + ERR_NOTEXTTOSEND(this->getEmitter().getUID()));
			break ;
		case 2:
			/* right numbers of params */
			std::string targetName = Parser::getParam(params, 0);
			Logger::debug("Privmsg target: " + targetName); 
			if (this->getServer()._entities.count(targetName) == 0)
			{
				/* target doesn't exist */
				Logger::debug("Privmsg: Target not found !");
					this->getServer()._sendMessage(this->getEmitter(), ERR_NOSUCHNICK(this->getEmitter().getUID(), targetName));
				return SUCCESS;
			}
			AEntity *target = this->getServer()._entities[targetName];
			if (target->getType() & Channel::value_type)
			{
				/* additional tests when target is a channel */
				Logger::debug("Privmsg: target is a channel");
				if (reinterpret_cast<Channel *>(target)->isRegistered(static_cast<Client&>(this->getEmitter())) == false)
				{
					Logger::debug("Privmsg: not in channel ! (" + this->getEmitter().getUID() + ")");
					this->getServer()._sendMessage(this->getEmitter(), ERR_CANNOTSENDTOCHAN(this->getEmitter().getUID(), target->getUID()));
					return SUCCESS;
				}
			}
			AEntity& emitter = this->getEmitter();
			Logger::info(this->getEmitter().getUID() + " relay a message from " + emitter.getUID() + " to " + target->getUID());
			std::string msg = emitter.getPrefix() + " PRIVMSG " + target->getUID() + " :" + Parser::getParam(params, 1);
			Logger::debug("MSG = " + msg);
			// send local
			this->getServer()._sendMessage(*target, msg, &this->getEmitter());

			//send relay
			this->getServer()._sendAllServers(msg, &executor);
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
