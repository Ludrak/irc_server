# include "IRCServer.hpp"

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