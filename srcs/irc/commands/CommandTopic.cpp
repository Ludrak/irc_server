#include "CommandTopic.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandTopic::CommandTopic(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandTopic::~CommandTopic()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
*/
uint					CommandTopic::operator()(NetworkEntity & executor, std::string params)
{
	int nb = Parser::nbParam(params);

	if (nb == 0)
	{
		this->getServer()._sendMessage(this->getEmitter(), ERR_NEEDMOREPARAMS(this->getEmitter().getUID(), "TOPIC"));
		return (SUCCESS);
	}
	else if (nb >= 1)
	{
		Channel *chan = NULL;
		std::string chan_name = Parser::getParam(params, 0);

		if (executor.getFamily() == SERVER_ENTITY_FAMILY && nb >= 2)
		{
			if (this->getServer()._channels.count(chan_name) == 0)
			{
				Logger::critical("Setting topic of an unknown channel from another server");
				return (ERROR);
			}
			chan = this->getServer()._channels[chan_name];
			chan->setTopic(Parser::getParam(params, 1));
		}
		else if (this->getEmitter().getType() & Client::value_type)
		{
			Client& client = static_cast<Client&>(this->getEmitter());
			for (std::list<Channel*>::const_iterator it = client.getChannels().begin(); it != client.getChannels().end(); ++it)
			{
				if ((*it)->getUID() == chan_name)
				{
					chan = *it;
					break;
				}
			}
			if (chan == NULL)
			{
				this->getServer()._sendMessage(this->getEmitter(), ERR_NOTONCHANNEL(this->getEmitter().getUID(), chan_name));
				return (SUCCESS);
			}
			else if (chan->getCreator()->getUID() != client.getUID() && (nb != 1 && !chan->hasMode(MODE_CHAN_TOPICOPONLY)))
			{
				this->getServer()._sendMessage(this->getEmitter(), ERR_CHANOPRIVSNEEDED(this->getEmitter().getUID(), chan_name));
				return (SUCCESS);
			}
			if (nb >= 2)
			{
				std::string new_topic = Parser::getParam(params, 1);
				chan->setTopic(new_topic);
			}
		}
		if (nb >= 2)
		{
			this->getServer()._sendAllServers(this->getEmitter().getPrefix() + "TOPIC " + chan->getUID() + " :" + chan->getTopic(), &executor);
	
			if (!chan->getTopic().empty())
				this->getServer()._sendMessage(*chan, RPL_TOPIC(this->getEmitter().getUID(), chan->getUID(), chan->getTopic()));
			else
				this->getServer()._sendMessage(*chan, RPL_NOTOPIC(this->getEmitter().getUID(), chan->getUID()));
		}
		else
		{
			if (!chan->getTopic().empty())
				this->getServer()._sendMessage(this->getEmitter(), RPL_TOPIC(this->getEmitter().getUID(), chan->getUID(), chan->getTopic()));
			else
				this->getServer()._sendMessage(this->getEmitter(), RPL_NOTOPIC(this->getEmitter().getUID(), chan->getUID()));
		}
		
		return (SUCCESS);
	}
	return SUCCESS;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandTopic::hasPermissions(AEntity & executor)
{
	(void) executor;
	return true;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */