#include "CommandJoin.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandJoin::CommandJoin(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandJoin::~CommandJoin()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
	Command: JOIN
	Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] )
				/ "0"
*/
uint					CommandJoin::operator()(NetworkEntity & executor, std::string params)
{
 	size_t nbParam = Parser::nbParam(params);
	if ( nbParam == 0)
	{
        this->getServer()._sendMessage(executor, ERR_NEEDMOREPARAMS(executor.getUID(), std::string("JOIN")));
        return SUCCESS;
    }
	std::list<std::string> channels(Parser::paramToList(Parser::getParam(params, 0)));

	ClientInfo*	client_info;
	if (this->getEmitter().getType() & RelayedClient::value_type)
	{
		client_info = static_cast<ClientInfo*>(static_cast<RelayedClient*>(&this->getEmitter()));
	}
	else if (this->getEmitter().getType() & Client::value_type)
	{
		client_info = static_cast<ClientInfo*>(static_cast<Client*>(&this->getEmitter()));
	}

	if (channels.size() == 1 && channels.front().compare("0") == 0)
	{
		client_info->leaveAllChannels();
		return SUCCESS;
	}
	else if (client_info->maxChannelAccessReached())
	{
		this->getServer()._sendMessage(this->getEmitter(), ERR_TOOMANYCHANNELS(this->getEmitter().getUID()));
		return SUCCESS;
	}

	std::list<std::string> keys(Parser::paramToList(Parser::getParam(params, 1)));

	std::list<std::string>::iterator itk = keys.begin();
	for (std::list<std::string>::iterator itc = channels.begin(); itc != channels.end(); ++itc)
	{
		if (itk != keys.end())
			Logger::debug("executor " + this->getEmitter().getUID() + ": ask to join channel: " + *itc + " with key: " + *itk++);
		else
			Logger::debug("executor " + this->getEmitter().getUID() + ": ask to join channel: " + *itc);

		//Channel don't exist
		if (this->getServer()._channels.count(*itc) == 0)
		{
			if (Parser::validChannelName(*itc) == false)
			{
				Logger::debug("Join: invalid channel name |" + *itc + "|");
				this->getServer()._sendMessage(this->getEmitter(), this->getEmitter().getPrefix() + ERR_NOSUCHCHANNEL(this->getEmitter().getUID(), *itc));
				continue ;
			}
			if (client_info->maxChannelAccessReached())
			{
				this->getServer()._sendMessage(this->getEmitter(), this->getEmitter().getPrefix() + ERR_TOOMANYCHANNELS(this->getEmitter().getUID()));
				return (SUCCESS);
			}


			Channel* new_chan =  new Channel(*itc);
			Logger::info("Creating a new channel: " + new_chan->getUID());
			//REVIEW should only be gived to safer channel
			if (client_info->joinChannel(*new_chan))
			{
				Logger::critical("Cannot join freshly created channel: " + new_chan->getUID());
				delete new_chan;
				return (ERROR);
			}

			Logger::debug("Adding client " + this->getEmitter().getUID() + " to channel " + new_chan->getUID());
			uint ret = new_chan->addClient(this->getEmitter());
			if (ret != SUCCESS)
			{
				Logger::critical("Send error message to user + delete new_chan + remove new_chan from client_info._channels");
				return ret;
			}
			new_chan->setCreator(this->getEmitter());
			this->getServer()._channels.insert(std::make_pair(new_chan->getUID(), new_chan));
			this->getServer()._entities.insert(std::make_pair(new_chan->getUID(), new_chan));

			this->getServer()._sendMessage(*new_chan, this->getEmitter().getPrefix() + "JOIN " + new_chan->getUID());
			this->getServer()._sendAllServers(this->getEmitter().getPrefix() + "JOIN " + new_chan->getUID(), &executor);
			this->getServer()._sendMessage(this->getEmitter(), this->getServer().getPrefix() + RPL_NOTOPIC(this->getEmitter().getUID(), new_chan->getUID()));
			continue ;
		}
		//Channel Exist
		Channel* chan = this->getServer()._channels[*itc];
		if ((chan->getType() & Channel::value_type) == false)
			this->getServer()._sendMessage(this->getEmitter(), ERR_NOSUCHCHANNEL(this->getEmitter().getUID(), *itc));
		else
		{
			if (chan->isRegistered(this->getEmitter()) == false)
			{
				Logger::info("Adding " + this->getEmitter().getUID() + " on channel: " + *itc);
				uint err = client_info->joinChannel(*chan);
				switch (err)
				{
					case 405:
						this->getServer()._sendMessage(this->getEmitter(), ERR_TOOMANYCHANNELS(this->getEmitter().getUID()));
						continue ;
					case 462:
						this->getServer()._sendMessage(this->getEmitter(), ERR_ALREADYREGISTRED(this->getEmitter().getUID()));
						continue ;
					case 471:
						this->getServer()._sendMessage(this->getEmitter(), ERR_CHANNELISFULL(chan->getUID()));
						continue ;
					case SUCCESS:
						chan->addClient(this->getEmitter());
						break ;
					default:
						Logger::critical("Join: return error: " + ntos(err));
				}
				std::string message = this->getEmitter().getPrefix() + "JOIN " + chan->getUID();
				this->getServer()._sendMessage(*chan, message, &this->getServer());
				this->getServer()._sendAllServers(this->getEmitter().getPrefix() + "JOIN " + chan->getUID(), &executor);

				if (this->getEmitter().getType() & NetworkEntity::value_type)
				{
					if (!chan->getTopic().empty())
					{
						this->getServer()._sendMessage(this->getEmitter(), this->getServer().getPrefix() +
												RPL_TOPIC(this->getEmitter().getUID(), chan->getUID(), chan->getTopic()));
					}
					else
					{
						this->getServer()._sendMessage(this->getEmitter(), this->getServer().getPrefix() +
												RPL_NOTOPIC(this->getEmitter().getUID(), chan->getUID()));
					}
					// REVIEW add @ or + for operator and voiced channel user modes respectivly
					std::string name_list;
					for (std::list<AEntity*>::iterator it = chan->clientBegin(); it != chan->clientEnd(); ++it)
						name_list += (*it)->getUID() + " ";	
					this->getServer()._sendMessage(this->getEmitter(), this->getServer().getPrefix() +
												RPL_NAMREPLY(this->getEmitter().getUID(), "= " + chan->getUID(), name_list));
					this->getServer()._sendMessage(this->getEmitter(), this->getServer().getPrefix() +
												RPL_ENDOFNAMES(this->getEmitter().getUID(), chan->getUID()));
				}
			}
		}
	}
	return (SUCCESS);
}





/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandJoin::hasPermissions(AEntity & executor)
{
	if (executor.getFamily() == CLIENT_ENTITY_FAMILY || executor.getFamily() == SERVER_ENTITY_FAMILY)
	    return true;
    return (false);
}

/* ************************************************************************** */