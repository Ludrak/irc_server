#include "CommandNick.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandNick::CommandNick() : ACommand()
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandNick::~CommandNick()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

uint					CommandNick::operator()(AEntity & executor, std::string params)
{
	( void ) executor;
	( void ) params;
	// Logger::debug("<" + ntos(executor.getStream().getSocket()) + "> Command<NICK> with args: " + params );
	// std::string nick = Parser::getParam(params, 0);
	// if (executor.getPassword() != this->_password)
	// {
	// 	//TODO handle this error without disconnecting
	// 	this->disconnect(executor.getStream()); // kickUser
	// 	return SUCCESS;
	// }
	// else if (Parser::nbParam(cmd) != 1) //REVIEW normalement si 2 param venant d'un user, commande ignoré
	// 	return this->_reply(executor, ERR_NONICKNAMEGIVEN); //TODO handle Nick other params coming from server
	// else if (Parser::validNickname(nick) == false)
	// 	return this->_reply(executor, ERR_ERRONEUSNICKNAME, nick);
	// else if (this->_ircClients.count(nick) != 0)
	// 	return this->_reply(executor, ERR_NICKNAMEINUSE, nick);
	// else if (this->_ircClients.count(executor.getNickname()) != 0)
	// {
	// 	Logger::info("Renaming " + executor.getNickname() + " to " + nick);
	// 	this->_ircClients.erase(executor.getNickname());
	// 	this->_ircClients.insert(std::make_pair(nick, &executor));
	// }
	// client.setNickname(nick);
	return SUCCESS;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandNick::hasPermissions(AEntity & executor)
{
	//TODO implement right for NICK
	( void) executor;
	return SUCCESS;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */