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
	// this->_sendMessage(client, client.getNickname() + cmd);
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
			client.joinChannel(*new_chan);
			this->_ircClients.insert(std::make_pair(*itc, new_chan));
			this->_sendMessage(client, ":" + client.getNickname() + " test@sender-server JOIN :" + new_chan->getNickname());
			// this->_
			// this->_reply(client, SUCCESS);
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
				client.joinChannel(*chan);
				//TODO check  return value
				//TODO send JOIN as success
				Package pkg(this->_protocol, this->_protocol.format(":" + client.getNickname() + " test@sender-server JOIN :" + aChannel->getNickname()));
				chan->broadcastPackage(pkg);
			}
		}
	}
	return SUCCESS;
}
