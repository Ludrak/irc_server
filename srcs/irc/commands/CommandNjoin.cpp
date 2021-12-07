#include "CommandNjoin.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandNjoin::CommandNjoin(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandNjoin::~CommandNjoin()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
Command: NJOIN
   	Parameters: <channel> [ "@@" / "@" ] [ "+" ] <nickname>
                         *( "," [ "@@" / "@" ] [ "+" ] <nickname> )
*/
uint					CommandNjoin::operator()(NetworkEntity & executor, std::string params)
{
	//REVIEW clean commentss
    size_t nbParam = Parser::nbParam(params);
	if ( nbParam < 2)
	{
        this->getServer()._sendMessage(this->getEmitter(), this->getServer().getPrefix() + ERR_NEEDMOREPARAMS(this->getEmitter().getUID(), std::string("NJOIN")));
        return SUCCESS;
    }
	std::string channel(Parser::getParam(params, 0));
	if (Parser::validChannelName(channel) == false)
	{
		this->getServer()._sendMessage(this->getEmitter(), this->getServer().getPrefix() + ERR_NOSUCHCHANNEL(this->getEmitter().getUID(), channel));
		return SUCCESS;	
	}
	Channel *new_chan;
	if (this->getServer()._channels.count(channel) == 0)
	{
		new_chan = new Channel(channel);
		this->getServer()._entities.insert(std::make_pair(channel, new_chan));
		this->getServer()._channels.insert(std::make_pair(channel, new_chan));
		Logger::info("Inserting a new channel: " + channel);
	}
	else {
		new_chan = this->getServer()._channels[channel];
		Logger::info("Appening to channel: " + channel);
	}
    std::list<std::string> nickList(Parser::paramToList(Parser::getParam(params, 1)));
	/* send to clients in channel the new registrations */
	for (std::list<std::string>::iterator it = nickList.begin(); it != nickList.end(); ++it)
	{
		std::string nick(*it);
		bool creator = false;
		if (nick.size() > 2 && nick.compare(0, 2, "@@") == 0)
		{
			creator = true;
			nick = nick.substr(2);
			// Logger::debug("Chan creator: " + nick);
		}
		// Logger::debug("creator: " + ntos(creator));
		if (this->getServer()._clients.count(nick) == 0)
		{
			Logger::critical("Received an unknown nickname: " + nick);
			continue ;
		}
		AEntity* user = this->getServer()._clients[nick];
		Logger::debug("sending JOIN to: " + new_chan->getUID());
		this->getServer()._sendMessage(*new_chan, user->getPrefix() + "JOIN " + new_chan->getUID());	
	}
	/* add new_clients to channel */
	for (std::list<std::string>::iterator it = nickList.begin(); it != nickList.end(); ++it)
	{
		bool creator = false;
		std::string nick(*it);
		if (nick.size() > 2 && nick.compare(0, 2, "@@") == 0)
		{
			creator = true;
			nick = nick.substr(2);
			// Logger::debug("Chan creator: " + nick);
		}
		// Logger::debug("creator: " + ntos(creator));
		if (this->getServer()._clients.count(nick) == 0)
		{
			Logger::critical("Received an unknown nickname: " + nick);
			continue ;
		}	
		AEntity* user = this->getServer()._clients[nick];
		ClientInfo* info;
		new_chan->setCreator(*user);
		if (user->getType() & Client::value_type)
		{
			info = &static_cast<Client&>(*user);
			Logger::error("Njoin: try to add a local user the the channel");
		}
		else
			info = &static_cast<RelayedClient&>(*user);

		if (info->joinChannel(*new_chan))
		{
			Logger::critical("Njoin: Cannot join channel: " + new_chan->getUID());
			return (ERROR);
		}
		uint ret = new_chan->addClient(*user);
		if (ret != SUCCESS)
			return ret;
	}
	// Logger::error("End adding new clients");
	this->getServer()._sendAllServers(this->getEmitter().getPrefix() + "NJOIN " + params, &executor);
	// Logger::error("forward NJOIN to all servers");
	this->getServer()._printServerState();
	// Logger::error("out of NJOIN");
	return SUCCESS;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandNjoin::hasPermissions(AEntity & executor)
{
	if (executor.getFamily() == SERVER_ENTITY_FAMILY)
	    return true;
    return (false);
}

/* ************************************************************************** */