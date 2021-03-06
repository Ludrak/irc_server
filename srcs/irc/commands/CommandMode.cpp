#include "CommandMode.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandMode::CommandMode(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandMode::~CommandMode()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
User:
	Command: MODE
	Parameters: <nickname>
				*( ( "+" / "-" ) *( "i" / "w" / "o" / "O" / "r" ) )
Channel:
	Command: MODE
	Parameters: <channel> *( ( "-" / "+" ) *<modes> *<modeparams> )
*/
uint					CommandMode::operator()(NetworkEntity & executor, std::string params)
{
	uint nbParam = Parser::nbParam(params);
	if (nbParam == 0)
	{
		this->getServer()._sendMessage(executor, ERR_NEEDMOREPARAMS(executor.getUID(), std::string("MODE")));
		return SUCCESS;
	}
	std::string uid = Parser::getParam(params, 0);
	if (this->getServer()._entities.count(uid) == 0)
	{
		this->getServer()._sendMessage(executor, ERR_NOSUCHNICK(executor.getUID(), uid));
		return SUCCESS;
	}
	else if (this->getServer()._channels.count(uid) == 1)
	{
		if (nbParam < 2)
		{
			//this->getServer()._sendMessage(executor, ERR_NEEDMOREPARAMS(executor.getUID(), std::string("MODE")));
			this->getServer()._sendMessage(executor, RPL_UMODEIS(this->getEmitter().getUID(), static_cast<Client &>(this->getEmitter()).getModeString()));
			return SUCCESS;
		}		
		this->modeForChannel(nbParam, executor, params);
	}
	else
		this->modeForUser(executor, uid, Parser::getParam(params, 1));
	return SUCCESS;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandMode::hasPermissions(AEntity & executor)
{
	if (executor.getType() & UnRegisteredConnection::value_type)
		return false;
	else if (this->getEmitter().getFamily() == SERVER_ENTITY_FAMILY)
	{
		Logger::debug("Access denied because of server type");
		return false;
	}
	return true;
}

void				CommandMode::modeForUser(NetworkEntity & executor, std::string uid, std::string mode)
{
	if (this->getEmitter().getUID() != uid)
	{
		Logger::debug("Njoin: " + this->getEmitter().getUID() + " Change mode for the other user: " + uid);
		this->getServer()._sendMessage(executor, this->getServer().getPrefix() + ERR_USERSDONTMATCH(this->getEmitter().getUID()));
		return ;
	}
	else if (mode.empty())
	{
		//Query about mode of client
		if (executor.getType() & Client::value_type)
			this->getServer()._sendMessage(executor, RPL_UMODEIS(this->getEmitter().getUID(), static_cast<Client &>(this->getEmitter()).getModeString()));
		else
			Logger::critical("Mode: unhandled type");
		return ;
	}
	else
	{
		Logger::warning("TODO handle adding a user mode");
	}
	return ;
}

void				CommandMode::modeForChannel(uint nbParam, NetworkEntity & executor, std::string params)
{
	Logger::warning("TODO mode for channel");
	//TODO Check if mode is valid
	
	std::string uid = Parser::getParam(params, 0);
	if (this->getServer()._channels.count(uid) == 0)
	{
		this->getServer()._sendMessage(executor, ERR_NOSUCHCHANNEL(executor.getUID(), uid));
		return ;
	}
	std::string mode = Parser::getParam(params, 2);
	Channel *chan = this->getServer()._channels[uid];
	if (mode.find("O") != std::string::npos && nbParam >= 3)
		chan->setCreator(this->getEmitter());
	return ;
}