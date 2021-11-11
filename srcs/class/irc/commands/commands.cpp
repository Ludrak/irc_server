# include "IRCServer.hpp"

//TODO implement PART command


uint						IRCServer::_commandPASS(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<PASS> with args: " + cmd );
	if (client.isRegistered())
		return this->_reply(client, ERR_ALREADYREGISTRED);
	else if (cmd.empty())
		return this->_reply(client, ERR_NEEDMOREPARAMS, "PASS");
	client.setPassword(cmd);
	return SUCCESS;
}

uint						IRCServer::_commandNICK(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<NICK> with args: " + cmd );
	std::string nick = Parser::getParam(cmd, 0);
	if (client.getPassword() != this->_password)
	{
		//TODO handle this error without disconnecting
		this->disconnect(client.getStream()); // kickUser
		return SUCCESS;
	}
	else if (Parser::nbParam(cmd) != 1) //REVIEW normalement si 2 param venant d'un user, commande ignoré
		return this->_reply(client, ERR_NONICKNAMEGIVEN); //TODO handle Nick other params coming from server
	else if (Parser::validNickname(nick) == false)
		return this->_reply(client, ERR_ERRONEUSNICKNAME, nick);
	else if (this->_ircClients.count(nick) != 0)
		return this->_reply(client, ERR_NICKNAMEINUSE, nick);
	else if (this->_ircClients.count(client.getNickname()) != 0)
	{
		Logger::info("Renaming " + client.getNickname() + " to " + nick);
		this->_ircClients.erase(client.getNickname());
		this->_ircClients.insert(std::make_pair(nick, &client));
	}
	client.setNickname(nick);
	return SUCCESS;
}

uint						IRCServer::_commandUSER(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<USER> with args: " + cmd );
	if (client.isRegistered())
		return this->_reply(client, ERR_ALREADYREGISTRED);
	else if (Parser::nbParam(cmd) < 4)
		return this->_reply(client, ERR_NEEDMOREPARAMS, "USER");
	client.setUsername(Parser::getParam(cmd, 0));
	client.setDomaineName(Parser::getParam(cmd, 1));
	client.setServername(Parser::getParam(cmd, 2));
	client.setRealname(Parser::getParam(cmd, 3));
	this->_setRegistered(client);
	Logger::info("new user registered: " + client.getNickname());
	return SUCCESS;
}
//TODO Parser::get param => error including on space
//TODO implement channels here and list of nickname/channels
uint		IRCServer::_commandPRIVMSG(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<PRIVMSG> with args: " + cmd );
	Logger::info(client.getNickname() + "<PRIVMSG>: " + cmd);
	size_t nbParam = Parser::nbParam(cmd);
	Logger::debug("nb param = " + ntos(nbParam)); 
	if (nbParam == 0)
		return this->_reply(client, ERR_NORECIPENT, cmd);
	else if (nbParam == 1)
		return this->_reply(client, ERR_NOTEXTTOSEND);
	else if (nbParam > 2)
		return SUCCESS; //REVIEW I don't know what to do here
	std::string target_name = Parser::getParam(cmd, 0);
	Logger::debug("Param target = " + target_name); 
	if (this->_ircClients.count(target_name) == 0)
	{
		Logger::debug("Target not found"); 
		return this->_reply(client, ERR_NOSUCHNICK, target_name);
	}
	AEntity *target = this->_ircClients[target_name];
	if (target->getType() == AEntity::value_type_channel)
	{
		//target is channel
		if (reinterpret_cast<Channel *>(target)->isRegistered(client) == false)
		{
			return this->_reply(client, ERR_CANNOTSENDTOCHAN, target->getNickname());
		}
	}
	std::string prefix = "";
	std::string msg = ":" + client.getNickname() + "!server-ident@sender-server PRIVMSG " + target->getNickname() + " :" + Parser::getParam(cmd, 1);
	Logger::debug("to " + client.getNickname() + " send " + msg);
	if (target->getType() == AEntity::value_type_channel)
		this->_sendMessage(*target, msg, &client.getStream());
	else
		this->_sendMessage(*target, msg);
	return SUCCESS;
}

uint		IRCServer::_commandDESCRIBE(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<DESCRIBE> with args: " + cmd );
	// if (Parser::nbParam(cmd) != 2)
		// this->_sendMessage(client, client.getNickname() + cmd);
		//TODO implement it
	return SUCCESS;
}

//TODO handle prefix in cmd before sending cmd to command
uint		IRCServer::_commandMODEchannel(Client & client, Channel& target, size_t nbParam, std::string cmd)
{
	//TODO pass this as static function 
	//bascially handled for know
	std::string mode = Parser::getParam(cmd, 1);
	(void) nbParam;
	if (mode.size() == 1)
	{
		switch (mode[0])
		{
			case 'O':
			{
				//TODO check here if command 'O' come from a server
				// if (client.getType() == AEntity::value_type_server)
				if (target.getCreator() == NULL)
				{
					target.setCreator(client);
					Logger::info("Give 'channel creator' status to " + client.getNickname());
				}
				// else
					// this->_reply(client, RPL_UNIQOPIS);//TODO
				break;
			}
			case 'm':
			{
				Logger::debug("Toogle moderated on channel " + target.getNickname());
				target.toogleMode(M_MODERATED);
				break ;
			}
			default:
			{
				this->_reply(client, ERR_UNKNOWNMODE, mode, target.getNickname());
			}
		}
	}
	else if (mode.size() == 2 && (mode[0] == '+' || mode[0] == '-'))
	{
		//valid
		Logger::warning("valid but not handled yet MODE (channel)");
	}
	else
	{
		Logger::warning("Invalid MODE (" + mode + ") for channel");
		this->_reply(client, ERR_UNKNOWNMODE, mode, target.getNickname());
	}
	return SUCCESS;
}

uint		IRCServer::_commandMODEclient(Client & client, std::string cmd)
{
	Logger::warning("MODE (client) not supported yet");
	(void) client;
	(void) cmd;
	return SUCCESS;
}

uint		IRCServer::_commandMODE(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<MODE> with args: " + cmd );
	size_t nbParam = Parser::nbParam(cmd);
	if ( nbParam < 2)
		return this->_reply(client, ERR_NEEDMOREPARAMS, "MODE");
	std::string target_name = Parser::getParam(cmd, 0);
	if (this->_ircClients.count(target_name) == false)
		return this->_reply(client, ERR_NOSUCHNICK, target_name);

	AEntity* target = this->_ircClients[target_name];
	if (target->getType() == AEntity::value_type_channel )
		return _commandMODEchannel(client, *reinterpret_cast<Channel *>(target), nbParam, cmd);
	else if (target->getType() == AEntity::value_type_client )
		return _commandMODEclient(client, cmd);
	Logger::warning("Unhandled type in MODE command");
	return SUCCESS;

}

//TODO add paramToList to every needed
//TODO add command OP only for displaying server state 
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
