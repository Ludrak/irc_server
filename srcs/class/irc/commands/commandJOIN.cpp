# include "IRCServer.hpp"


//TODO add paramToList to every needed
//TODO add command OP only for displaying server state voir Command INFO
uint		IRCServer::_commandJOIN(Client & client, std::string cmd)
{
	//TODO handle all number of param
	//TODO handle key for a channel
	//TODO add a method get prefix to AEntity
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<JOIN> with args: " + cmd );
	size_t nbParam = Parser::nbParam(cmd);
	if ( nbParam == 0)
		return this->_reply(client, ERR_NEEDMOREPARAMS, "JOIN");
	std::list<std::string> channels(Parser::paramToList(Parser::getParam(cmd, 0)));
	if (channels.size() == 1 && channels.front().compare("0") == 0)
		return client.leaveAllChannels();
	if (client.isFull())
		return this->_reply(client, ERR_TOOMANYCHANNELS, client.getNickname());
	std::list<std::string> keys(Parser::paramToList(Parser::getParam(cmd, 1)));

	std::list<std::string>::iterator itk = keys.begin();
	for (std::list<std::string>::iterator itc = channels.begin(); itc != channels.end(); ++itc)
	{
		if (itk != keys.end())
			Logger::debug("client " + client.getNickname() + ": ask to join channel: " + *itc + " with key: " + *itk++);
		else
			Logger::debug("client " + client.getNickname() + ": ask to join channel: " + *itc);
		if (this->_ircClients.count(*itc) == false)
		{
			//Channel don't exist
			if ( Parser::validChannelName(*itc) == false)
			{
				this->_reply(client, ERR_NOSUCHCHANNEL, *itc);
				continue ;
			}
			Logger::info("Creating a new channel: " + *itc);
			Channel* new_chan =  new Channel(*itc);
			switch (client.joinChannel(*new_chan))
			{
				case ERR_CHANNELISFULL:
					this->_reply(client, ERR_CHANNELISFULL, new_chan->getNickname());
					
			}
			this->_ircClients.insert(std::make_pair(*itc, new_chan));
			this->_sendMessage(client, ":" + client.getNickname() + " test@sender-server JOIN :" + new_chan->getNickname());
			this->execute(client, "MODE " + new_chan->getNickname() +  " O " + client.getNickname());
			continue ;
		}
		AEntity* aChannel = this->_ircClients.find(*itc)->second;
		if (aChannel->getType() != AEntity::value_type_channel)
			this->_reply(client, ERR_NOSUCHCHANNEL, *itc);
		else
		{
			//Channel Exist
			Channel *chan = dynamic_cast<Channel*>(aChannel);
			if (chan->isRegistered(client) == false)
			{
				Logger::info("Adding " + client.getNickname() + " on channel: " + *itc);
				switch (client.joinChannel(*chan))
				{
					case ERR_CHANNELISFULL:
						return this->_reply(client, ERR_CHANNELISFULL, chan->getNickname());
				}
				Package pkg(this->_protocol, this->_protocol.format(":" + client.getNickname() + " test@sender-server JOIN :" + aChannel->getNickname()));
				chan->broadcastPackage(pkg);
			}
		}
	}
	return SUCCESS;
}