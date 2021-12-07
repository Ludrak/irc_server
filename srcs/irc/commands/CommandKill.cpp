#include "CommandKill.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandKill::CommandKill(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandKill::~CommandKill()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*

Command: KILL
	Parameters: <nickname> <comment>
*/
uint					CommandKill::operator()(NetworkEntity & executor, std::string params)
{
	(void) executor;
	AEntity &emitter = this->getEmitter();
	if (Parser::nbParam(params) < 2)
	{
		this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + ERR_NEEDMOREPARAMS(emitter.getUID(), "KILL"));
		return SUCCESS;
	}
	std::string nick = Parser::getParam(params, 0);
	if (emitter.getType() & Client::value_type && static_cast<Client&>(emitter).isServerOP() == false)
		this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + ERR_NOPRIVILEGES(emitter.getUID()));
	else if (this->getServer()._entities.count(nick) == 0)
		this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + ERR_NOSUCHNICK(emitter.getUID(), nick));
	else
	{
		AEntity *target = this->getServer()._entities[nick];
		if (target->getType() & Channel::value_type)
			this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + ERR_NOSUCHNICK(emitter.getUID(), nick));
		else if (target->getType() & (Server::value_type | Server::value_type_forward))
			this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + ERR_CANTKILLSERVER(emitter.getUID()));
		else
		{
			if (target->getType() & NetworkEntity::value_type)
			{
				/* Kill target */
				Package *explosive = new Package(
					this->getServer().getProtocol(),
					this->getServer().getProtocol().format(
						emitter.getPrefix() + "KILL " + params),
					&reinterpret_cast<NetworkEntity*>(target)->getStream(),
					true);
				Logger::warning("Sending KILL explosive message to " + target->getUID());
				this->getServer().sendPackage(explosive, reinterpret_cast<NetworkEntity*>(target)->getStream());
			}
			else
			{
				/* Forward request */
				this->getServer()._sendMessage(*target, emitter.getPrefix() + "KILL " + params);
			}
		}
	}
	return SUCCESS;
}


bool				CommandKill::hasPermissions(AEntity & executor)
{
	if (executor.getType() & UnRegisteredConnection::value_type)
		return false;
	return true;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */