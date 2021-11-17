#include "CommandNick.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandNick::CommandNick(CommandHandler & handler) : ACommand(handler)
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

uint					CommandNick::operator()(NetworkEntity & executor, std::string params)
{
	if (executor.getPassword() != this->getServer()._password)
		return SUCCESS;
	else if (executor.getFamily() == SERVER_ENTITY_FAMILY)
		return this->_nickFromServer(static_cast<Server &>(executor), params);
	else if (Parser::nbParam(params) == 0) 
	{
		this->getServer()._sendMessage(executor, ERR_NONICKNAMEGIVEN()); //TODO handle Nick other params coming from server
		return EXIT_FAILURE;
	}	
	std::string nick = Parser::getParam(params, 0);
	if (Parser::validNickname(nick) == false)
		this->getServer()._sendMessage(executor, ERR_ERRONEUSNICKNAME(nick));
	else if (this->getServer().alreadyInUseUID(nick) == true)
		this->getServer()._sendMessage(executor, ERR_NICKNAMEINUSE(nick));
	else if (executor.getType() & UnRegisteredConnection::value_type)
		return this->_nickFromUnregistered(static_cast<UnRegisteredConnection &>(executor), nick);
	else if (executor.getFamily() == CLIENT_ENTITY_FAMILY)
		return this->_nickFromClient(static_cast<Client &>(executor), nick);
	else
		Logger::critical("Nick: unknomn family - " + ntos(executor.getFamily()));
	return EXIT_FAILURE;
}



/*
** --------------------------------- METHODS ----------------------------------
*/


uint				CommandNick::_nickFromUnregistered(UnRegisteredConnection & executor, std::string & nick)
{
	executor.setUID(nick);
	return SUCCESS;
}

uint				CommandNick::_nickFromClient(Client & executor, std::string & nick)
{
	Logger::info("Renaming " + executor.getUID() + " to " + nick);
	executor.setUID(nick);
	this->getServer()._clients.erase(executor.getUID());
	this->getServer()._entities.erase(executor.getUID());
	this->getServer()._clients.insert(std::make_pair(nick, &executor));
	this->getServer()._entities.insert(std::make_pair(nick, &executor));
	return SUCCESS;
}

uint				CommandNick::_nickFromServer(Server & executor, std::string & params)
{
	Logger::error("UnImplemented nick from server");
	(void ) executor ;
	(void) params;
	return SUCCESS;
}

bool				CommandNick::hasPermissions(AEntity & executor)
{
	//TODO implement right for NICK
	( void) executor;
	return true;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */