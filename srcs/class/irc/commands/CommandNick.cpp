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
	{
		Logger::error("Nick: invalid password/" + executor.getPassword() + "/" + this->getServer()._password);
		return SUCCESS;
	}
	else if (executor.getFamily() == SERVER_ENTITY_FAMILY)
		return this->_nickFromServer(static_cast<Server &>(executor), params);
	else if (Parser::nbParam(params) == 0) 
	{
		Logger::warning("NickFromUnregistered");
		this->getServer()._sendMessage(executor, ERR_NONICKNAMEGIVEN()); //TODO handle Nick other params coming from server
		return EXIT_FAILURE;
	}	
	std::string nick = Parser::getParam(params, 0);
	if (Parser::validNickname(nick) == false)
	{
		this->getServer()._sendMessage(executor, ERR_ERRONEUSNICKNAME(nick));
		return SUCCESS;
	}
	else if (this->getServer().alreadyInUseUID(nick) == true)
	{
		this->getServer()._sendMessage(executor, ERR_NICKNAMEINUSE(nick));
		return SUCCESS;
	}
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
	Logger::warning("NickFromUnregistered");

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
	uint nbParams = Parser::nbParam(params);
	if (nbParams == 2)
	{
		Logger::warning("unhandled 2 arguments NICK between servers");
		return SUCCESS;
	}
	else if (nbParams != 7)
		return SUCCESS;
	std::string nick = Parser::getParam(params, 0);
	if (this->getServer().alreadyInUseUID(nick) == true)
	{
		//nick collision
		Logger::critical("Nick collision unhandled happened");
		return SUCCESS;
	}
	uint hopcount;
	try {
		hopcount = std::stoi(Parser::getParam(params, 1));
	}catch(const std::invalid_argument & e)
	{
		Logger::error("Non-number hopcount argument: " + Parser::getParam(params, 1));
		return SUCCESS;
	}
	std::string username = Parser::getParam(params, 2);
	if (Parser::validUser(username) == false)
	{
		Logger::debug("Invalid username, NICK command ignored");
		return SUCCESS;
	}
	std::string host = Parser::getParam(params, 3);
	//TODO check if valid hostname ?
	std::string serverToken = Parser::getParam(params, 4);
	//TODO check if valid severToken ?
	std::string umode = Parser::getParam(params, 5);
	(void) umode;
	//TODO do method that transform umode in form "+iw" to uint
	std::string realname = Parser::getParam(params, 6);
	RelayedClient *rClient = new RelayedClient(executor, hopcount + 1, nick, username, realname, 0, serverToken, host );
	this->getServer()._addClient(*rClient, NULL);
	Logger::info("New relayed client added: hop +" + ntos(hopcount));
	//Forward and backward this info
	std::stringstream reply_msg;
	reply_msg << "NICK " << rClient->getUID() << " " << rClient->getHopCount() << " " << rClient->getName() << " " << rClient->getHostname() << " " << rClient->getServerToken() << " " << rClient->getModeString() << " " << rClient->getRealname();
	this->getServer()._sendAllServers(reply_msg.str(), &executor);
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