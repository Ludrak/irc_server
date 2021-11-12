# include "IRCServer.hpp"

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
	std::stringstream ss;
	ss << ":" << client.getNickname() << "!server-ident@sender-server PRIVMSG " << target->getNickname() << " :" << Parser::getParam(cmd, 1);
	Logger::debug("to " + client.getNickname() + " send " + ss.str());
	if (target->getType() == AEntity::value_type_channel)
		this->_sendMessage(*target, ss, &client);
	else
		this->_sendMessage(*target, ss);
	return SUCCESS;
}