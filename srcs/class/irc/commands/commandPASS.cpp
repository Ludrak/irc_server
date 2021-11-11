# include "IRCServer.hpp"

/*
   Command: PASS
   Parameters: <password> <version> <flags> [<options>]
*/
//REVIEW		Unsupported/Unparsed param <options>
uint						IRCServer::_commandPASS(Client & client, std::string cmd)
{
	Logger::debug("<" + ntos(client.getStream().getSocket()) + "> Command<PASS> with args: " + cmd );
	if (client.isRegistered())
		return this->_reply(client, ERR_ALREADYREGISTRED, "PASS");
	switch (Parser::nbParam(cmd))
	{
		case 0:
			Logger::debug("- no param given");
			return this->_reply(client, ERR_NEEDMOREPARAMS, "PASS");
		case 3:
			client.setFlags(Parser::getParam(cmd, 2));
		case 2:
			client.setVersion(Parser::getParam(cmd, 1));
		case 1:
			client.setPassword(Parser::getParam(cmd, 0));
	}
	return SUCCESS;
}