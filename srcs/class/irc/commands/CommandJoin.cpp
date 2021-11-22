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

uint					CommandJoin::operator()(NetworkEntity & executor, std::string params)
{
    size_t nbParam = Parser::nbParam(params);
	if ( nbParam == 0)
	{
        this->getServer()._sendMessage(executor, ERR_NEEDMOREPARAMS(executor.getUID(), std::string("JOIN")));
        return SUCCESS;
    }
	std::list<std::string> channels(Parser::paramToList(Parser::getParam(params, 0)));
	if (channels.size() == 1 && channels.front().compare("0") == 0)
    {
        // TODO what to do on relayed client ?
		//TODO response: here it is exactly as doing PART for every Channel: implement it after PART
	    static_cast<Client &>(executor).leaveAllChannels();
        return SUCCESS;
    }
	else if (static_cast<Client &>(executor).maxChannelAccessReached())
	{
        // TODO same as above
        this->getServer()._sendMessage(executor, ERR_TOOMANYCHANNELS(executor.getUID()));
        return SUCCESS;
    }

    std::list<std::string> keys(Parser::paramToList(Parser::getParam(params, 1)));

	std::list<std::string>::iterator itk = keys.begin();
	for (std::list<std::string>::iterator itc = channels.begin(); itc != channels.end(); ++itc)
	{
		if (itk != keys.end())
			Logger::debug("executor " + executor.getUID() + ": ask to join channel: " + *itc + " with key: " + *itk++);
		else
			Logger::debug("executor " + executor.getUID() + ": ask to join channel: " + *itc);
		if (this->getServer()._channels.count(*itc) == false)
		{
			//Channel don't exist
			if ( Parser::validChannelName(*itc) == false)
			{
				Logger::debug("Join: invalid channel name |" + *itc + "|");
				this->getServer()._sendMessage(executor, ERR_NOSUCHCHANNEL(*itc));
				continue ;
			}
			Logger::info("Creating a new channel: " + *itc);
			Channel* new_chan =  new Channel(*itc);
			switch (reinterpret_cast<Client *>(&executor)->joinChannel(*new_chan))
			{
                case 1:
					this->getServer()._sendMessage(executor, ERR_TOOMANYCHANNELS(executor.getUID()));
                    continue ;
			    case 2:
					this->getServer()._sendMessage(executor, ERR_ALREADYREGISTRED());
                    continue ;
	            case 4:
					this->getServer()._sendMessage(executor, ERR_CHANNELISFULL(new_chan->getUID()));
                    continue ;
				case SUCCESS:
					break ;
            }
            // TODO _addChannel()
			this->getServer()._channels.insert(std::make_pair(*itc, new_chan));
            this->getServer()._entities.insert(std::make_pair(*itc, new_chan));

            // TODO handle prefix
			std::stringstream ss;
			ss <<  ":" << executor.getUID() << " test@sender-server JOIN :" << new_chan->getUID();
			this->getServer()._sendMessage(executor, ss);
			this->getHandler().handle(executor, "MODE " + new_chan->getUID() +  " O " + executor.getUID());
			continue ;
		}
		AEntity* aChannel = this->getServer()._channels[*itc];
		if ((aChannel->getType() & Channel::value_type) == false)
			this->getServer()._sendMessage(executor, ERR_NOSUCHCHANNEL(*itc));
		else
		{
			//Channel Exist
			Channel *chan = dynamic_cast<Channel*>(aChannel);
			if (chan->isRegistered(static_cast<Client &>(executor)) == false)
			{
				Logger::info("Adding " + executor.getUID() + " on channel: " + *itc);
				switch (static_cast<Client &>(executor).joinChannel(*chan))
				{
					case 1:
						this->getServer()._sendMessage(executor, ERR_CHANNELISFULL(chan->getUID()));
                        return SUCCESS;
                	case 2:
					    this->getServer()._sendMessage(executor, ERR_CHANNELISFULL(chan->getUID()));
                        return SUCCESS;
				}
				Package pkg(this->getServer().getProtocol(), this->getServer().getProtocol().format(":" + executor.getUID() + " test@sender-server JOIN :" + aChannel->getUID()));
				chan->broadcastPackage(pkg);
			}
		}
	}
	return SUCCESS;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandJoin::hasPermissions(AEntity & executor)
{
	if (executor.getFamily() == CLIENT_ENTITY_FAMILY)
	    return true;
    return (false);
}

/* ************************************************************************** */