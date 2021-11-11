# include "IRCServer.hpp"

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