# include "IRCServer.hpp"


//TODO add paramToList to every needed
//TODO add command OP only for displaying server state voir Command INFO
uint		IRCServer::_commandJOIN(AEntity & executor, std::string params)
{
	//TODO handle all number of param
	//TODO handle key for a channel
	//TODO add a method get prefix to AEntity
	Logger::debug("<" + ntos(executor.getStream().getSocket()) + "> Command<JOIN> with args: " + params );
	size_t nbParam = Parser::nbParam(params);
	if ( nbParam == 0)
		return this->_reply(executor, ERR_NEEDMOREPARAMS, "JOIN");
	std::list<std::string> channels(Parser::paramToList(Parser::getParam(params, 0)));
	if (channels.size() == 1 && channels.front().compare("0") == 0)
		return executor.leaveAllChannels();
	if (executor.isFull())
		return this->_reply(executor, ERR_TOOMANYCHANNELS, executor.getNickname());
	std::list<std::string> keys(Parser::paramToList(Parser::getParam(params, 1)));

	std::list<std::string>::iterator itk = keys.begin();
	for (std::list<std::string>::iterator itc = channels.begin(); itc != channels.end(); ++itc)
	{
		if (itk != keys.end())
			Logger::debug("executor " + executor.getNickname() + ": ask to join channel: " + *itc + " with key: " + *itk++);
		else
			Logger::debug("executor " + executor.getNickname() + ": ask to join channel: " + *itc);
		if (this->_ircClients.count(*itc) == false)
		{
			//Channel don't exist
			if ( Parser::validChannelName(*itc) == false)
			{
				this->_reply(executor, ERR_NOSUCHCHANNEL, *itc);
				continue ;
			}
			Logger::info("Creating a new channel: " + *itc);
			Channel* new_chan =  new Channel(*itc);
			switch (executor.joinChannel(*new_chan))
			{
				case ERR_CHANNELISFULL:
					this->_reply(executor, ERR_CHANNELISFULL, new_chan->getNickname());
					
			}
			this->_ircexecutors.insert(std::make_pair(*itc, new_chan));
			std::stringstream ss;
			ss <<  ":" << executor.getNickname() << " test@sender-server JOIN :" << new_chan->getNickname();
			this->_sendMessage(executor, ss);
			this->execute(executor, "MODE " + new_chan->getNickname() +  " O " + executor.getNickname());
			continue ;
		}
		AEntity* aChannel = this->_ircClients.find(*itc)->second;
		if (aChannel->getType() != AEntity::value_type_channel)
			this->_reply(executor, ERR_NOSUCHCHANNEL, *itc);
		else
		{
			//Channel Exist
			Channel *chan = dynamic_cast<Channel*>(aChannel);
			if (chan->isRegistered(executor) == false)
			{
				Logger::info("Adding " + executor.getNickname() + " on channel: " + *itc);
				switch (executor.joinChannel(*chan))
				{
					case ERR_CHANNELISFULL:
						return this->_reply(executor, ERR_CHANNELISFULL, chan->getNickname());
				}
				Package pkg(this->_protocol, this->_protocol.format(":" + executor.getNickname() + " test@sender-server JOIN :" + aChannel->getNickname()));
				chan->broadcastPackage(pkg);
			}
		}
	}
	return SUCCESS;
}