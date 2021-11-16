# include "IRCServer.hpp"

uint		IRCServer::_commandPRIVMSG(AEntity & executor, std::string params)
{
	Logger::debug("<" + ntos(executor.getStream().getSocket()) + "> Command<PRIVMSG> with args: " + params );
	Logger::info(executor.getNickname() + "<PRIVMSG>: " + params);
	size_t nbParam = Parser::nbParam(params);
	Logger::debug("nb param = " + ntos(nbParam)); 
	if (nbParam == 0)
		return this->_reply(executor, ERR_NORECIPENT, params);
	else if (nbParam == 1)
		return this->_reply(executor, ERR_NOTEXTTOSEND);
	else if (nbParam > 2)
		return SUCCESS; //REVIEW I don't know what to do here
	std::string target_name = Parser::getParam(params, 0);
	Logger::debug("Param target = " + target_name); 
	if (this->_ircClient.count(target_name) == 0)
	{
		Logger::debug("Target not found"); 
		return this->_reply(executor, ERR_NOSUCHNICK, target_name);
	}
	AEntity *target = this->_ircClient[target_name];
	if (target->getType() == AEntity::value_type_channel)
	{
		//target is channel
		if (reinterpret_cast<Channel *>(target)->isRegistered(executor) == false)
		{
			return this->_reply(executor, ERR_CANNOTSENDTOCHAN, target->getNickname());
		}
	}
	std::string prefix = "";
	std::stringstream ss;
	ss << ":" << executor.getNickname() << "!server-ident@sender-server PRIVMSG " << target->getNickname() << " :" << Parser::getParam(params, 1);
	Logger::debug("to " + executor.getNickname() + " send " + ss.str());
	if (target->getType() == AEntity::value_type_channel)
		this->_sendMessage(*target, ss, &executor);
	else
		this->_sendMessage(*target, ss);
	return SUCCESS;
}