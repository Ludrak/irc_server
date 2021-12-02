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
	}
    std::list<std::string> nickList(Parser::paramToList(Parser::getParam(params, 1)));
	
	for (std::list<std::string>::iterator it = nickList.begin(); it != nickList.end(); ++it)
	{
		std::string nick(*it);
		if (this->getServer()._entities.count(nick) == 0)
			continue ;
		new_chan->; this->getServer()._entities[nick];
		

	}

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