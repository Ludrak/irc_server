# include "IRCServer.hpp"

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