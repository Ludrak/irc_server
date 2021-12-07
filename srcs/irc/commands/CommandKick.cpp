
#include "CommandKick.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandKick::CommandKick(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandKick::~CommandKick()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
Command: KICK
   Parameters: <channel> *( "," <channel> ) <user> *( "," <user> )
               [<comment>]
*/
uint					CommandKick::operator()(NetworkEntity & executor, std::string params)
{
	(void) executor;
	AEntity &emitter = this->getEmitter();
	uint nbParam = Parser::nbParam(params);
	Logger::info("Param nb: " + ntos(nbParam));
	if ( nbParam < 2)
	{
		Logger::debug("Kick: nbParam(" + ntos(nbParam) + ")");
		this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + ERR_NEEDMOREPARAMS(emitter.getUID(), "KICK"));
		return SUCCESS;
	}
	std::list<std::string> chanList = Parser::paramToList(Parser::getParam(params, 0));
	std::string rawUserList(Parser::getParam(params, 1));
	Logger::debug("Raw userlist: <" + rawUserList + ">");
	std::list<std::string> userList = Parser::paramToList(rawUserList);
	if (chanList.size() != 1 && userList.size() != chanList.size())
	{
		Logger::debug("Kick: nbChan(" + ntos(chanList.size()) + ") nbUser(" + ntos(userList.size()) + ")");
		this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + ERR_NEEDMOREPARAMS(emitter.getUID(), "KICK"));
		return SUCCESS;
	}
	if (chanList.size() == 1)
	{
		/* One chan, many users */
		std::string chanName = chanList.front();
		if (this->getServer()._channels.count(chanName) == 0)
		{
			this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + ERR_NOSUCHCHANNEL(emitter.getUID(), chanName));
			return SUCCESS;
		}
		Channel* chan = this->getServer()._channels[chanName];
		if (chan->isRegistered(emitter) == false)
		{
			this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + ERR_NOTONCHANNEL(emitter.getUID(), chanName));
			return SUCCESS;
		}
		else if (emitter.getFamily() == CLIENT_ENTITY_FAMILY)
		{
			/* REVIEW implement chan operator? */
			/* Check emitter privileges on that channel */
			ClientInfo *cInfo = NULL;
			if (emitter.getType() & RelayedClient::value_type)
			{
				RelayedClient& client = static_cast<RelayedClient&>(emitter);
				cInfo = & client;
			}
			else
			{
				Client& client = static_cast<Client&>(emitter);
				cInfo = & client;
			}
			if (cInfo->isServerOP() == false)
			{
				this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + ERR_CHANOPRIVSNEEDED(emitter.getUID(), chanName));
				return SUCCESS;
			}
		}
		AEntity *user = NULL;
		for (std::list<std::string>::iterator it = userList.begin(); it != userList.end(); ++it)
		{
			if (this->getServer()._clients.count(*it) == false
			|| (user = this->getServer()._clients[*it]) == NULL
			|| chan->isRegistered(*user) == false)
			{
				this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + ERR_USERNOTINCHANNEL(emitter.getUID(), *it, chanName));
				continue ;
			}
			/* User in Channel: kick it */
			Logger::info("<" + emitter.getUID() + "> kick <" + user->getUID() + "> from " + chan->getUID());
			std::string kickMessage("PART " + chan->getUID() + " :");
			if (user->getType() & Client::value_type)
				kickMessage = reinterpret_cast<Client*>(user)->getPrefix() + kickMessage;
			else if (user->getType() & RelayedClient::value_type)
				kickMessage = reinterpret_cast<RelayedClient*>(user)->getPrefix() + kickMessage;
			else
				kickMessage = user->getPrefix() + kickMessage;
			if (nbParam >= 3)
				kickMessage += Parser::getParam(params, 2);
			else
				kickMessage += "kicked...";
			this->getHandler().handle(executor, kickMessage);
		}
		return SUCCESS;
	}
	else
	{
		/* as many chan given as users */
		//TODO to implement
		Logger::critical("One user by chan kick mode to do");
	}
	return SUCCESS;
}


bool				CommandKick::hasPermissions(AEntity & executor)
{
	if (executor.getType() & UnRegisteredConnection::value_type)
		return false;
	return true;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */