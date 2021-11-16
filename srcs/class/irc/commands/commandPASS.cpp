# include "IRCServer.hpp"

/*
   Command: PASS
   Parameters: <password> <version> <flags> [<options>]
*/
//REVIEW		Unsupported/Unparsed param <options>
uint						IRCServer::_commandPASS(AEntity & executor, std::string params)
{
	Logger::debug("<" + ntos(executor.getStream().getSocket()) + "> Command<PASS> with args: " + params );
	if (executor.isRegistered())
		return this->_reply(executor, ERR_ALREADYREGISTRED, "PASS");
	switch (Parser::nbParam(params))
	{
		case 0:
			Logger::debug("- no param given");
			return this->_reply(executor, ERR_NEEDMOREPARAMS, "PASS");
		case 3:
			executor.setFlags(Parser::getParam(params, 2));
		case 2:
			executor.setVersion(Parser::getParam(params, 1));
		case 1:
			executor.setPassword(Parser::getParam(params, 0));
	}
	return SUCCESS;
}