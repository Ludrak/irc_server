# include "IRCServer.hpp"

uint						IRCServer::_commandUSER(AEntity & executor, std::string params)
{
	//TOFIX: can register without nickname
	Logger::debug("<" + ntos(executor.getStream().getSocket()) + "> Command<USER> with args: " + params );
	if (executor.isRegistered())
		return this->_reply(executor, ERR_ALREADYREGISTRED, "USER");
	else if (Parser::nbParam(params) < 4)
		return this->_reply(executor, ERR_NEEDMOREPARAMS, "USER");
	executor.setUsername(Parser::getParam(params, 0));
	executor.setDomaineName(Parser::getParam(params, 1));
	executor.setServername(Parser::getParam(params, 2));
	executor.setRealname(Parser::getParam(params, 3));
	this->_setRegistered(executor, Client::value_type_client);
	Logger::info("new user registered: " + executor.getNickname());
	return SUCCESS;
}