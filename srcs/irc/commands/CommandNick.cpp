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

//TODO Improve by adding an except list to the possible nicknames (ex: RFC forbid `anonymous` as a valid nickname)

/*
User:
	Command: NICK
	Parameters: <nickname>
Server:
	Command: NICK
	Parameters: <nickname> <hopcount> <username> <host> <servertoken>
				<umode> <realname>
*/
uint					CommandNick::operator()(NetworkEntity & executor, std::string params)
{
	AEntity& emitter = this->getEmitter();
	/* Check password protection */
	if (this->getServer()._password != "" && executor.getPassword() != this->getServer()._password)
	{
			//REVIEW Do we need to send an ERROR message?
			Logger::error("Nick: invalid password/" + executor.getPassword() + "/" + this->getServer()._password);
			this->getServer().disconnect( executor.getStream());
			return SUCCESS;
			// Old version below | now we kick everybody with a bad password
			//
			// if (executor.getPassword().empty() && executor.getType() & UnRegisteredConnection::value_type)
			// {
			// 	Logger::error(std::string("Connection without password: kicking ") + ntos(executor.getStream().getSocket()) + "@" + executor.getStream().getHost() + " (" + executor.getStream().getIP() + ") ");
			// 	this->getServer().disconnect( executor.getStream());
			// }
	}
	else if (emitter.getFamily() == SERVER_ENTITY_FAMILY)
		return this->_nickFromServer(static_cast<Server &>(executor), params);
	
	/* Naming a client */
	if (Parser::nbParam(params) == 0) 
	{
		this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + ERR_NONICKNAMEGIVEN(emitter.getUID()));
		return SUCCESS;
	}	
	std::string nick = Parser::getParam(params, 0);
	if (Parser::validNickname(nick) == false)
	{
		this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + ERR_ERRONEUSNICKNAME(emitter.getUID(), nick));
		return SUCCESS;
	}
	else if (this->getServer().alreadyInUseUID(nick) == true)
	{
		if (executor.getType() & UnRegisteredConnection::value_type)
		{
			/* Kick an unregisteredConnection that try a nickname already in use */
			Package *explosive = new Package(
				this->getServer().getProtocol(),
				this->getServer().getProtocol().format(
					this->getServer().getPrefix() + ERR_NICKNAMEINUSE(nick, nick)),
				&executor.getStream(),
				true);
			Logger::warning("Send explosive package to executor (if not usefull remove this message)");
			this->getServer().sendPackage(explosive, executor.getStream());
		}
		else
			this->getServer()._sendMessage(emitter, this->getServer().getPrefix() + ERR_NICKNAMEINUSE(emitter.getUID(), nick));
		return SUCCESS;
	}
	else if (executor.getType() & UnRegisteredConnection::value_type)
		return this->_nickFromUnregistered(static_cast<UnRegisteredConnection &>(executor), nick);
	else
		return this->_renameClient(executor, emitter, nick);
	return EXIT_FAILURE;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


uint				CommandNick::_nickFromUnregistered(UnRegisteredConnection & executor, std::string & nick)
{
	Logger::debug("set nick " + nick + " to " + executor.getUID() + "(" + executor.getStream().getHost() + ")");
	executor.setUID(nick);
	return SUCCESS;
}

uint				CommandNick::_renameClient(NetworkEntity & executor, AEntity & emitter, std::string & nick)
{
	std::string	oldPrefix(emitter.getPrefix());
	Logger::info("Renaming " + emitter.getUID() + " to " + nick);
	this->getServer()._clients.erase(emitter.getUID());
	this->getServer()._entities.erase(emitter.getUID());
	this->getServer()._sendMessage( emitter, emitter.getPrefix() + "NICK " + nick);

	emitter.setUID(nick);
	this->getServer()._clients.insert(std::make_pair(nick, &emitter));
	this->getServer()._entities.insert(std::make_pair(nick, &emitter));
	this->getServer()._sendAllServers( oldPrefix + "NICK " + nick, &executor);
	return SUCCESS;
}

uint				CommandNick::_nickFromServer(Server & executor, std::string & params)
{
	if (Parser::nbParam(params) != 7)
		return SUCCESS;
	std::string nick = Parser::getParam(params, 0);
	if (this->getServer().alreadyInUseUID(nick) == true)
	{
		Logger::warning("Nick collision happend: " + nick);
		/* nick collision */
		std::string killRequest(this->getServer().getPrefix() + "KILL " + nick + " :Nick collision occured");
		/* Delete local reference */
		this->getHandler().handle(executor, killRequest);
		/* Delete remote reference */
		this->getServer()._sendAllServers(killRequest);
		return SUCCESS;
	}
	uint hopcount = 0;
	std::istringstream is(Parser::getParam(params, 1));
	is >> hopcount;
	if (hopcount == 0)
	{
		Logger::debug("Nick: Invalid hopcount. Command ignored");
		return SUCCESS;
	}
	std::string username = Parser::getParam(params, 2);
	if (Parser::validUser(username) == false)
	{
		Logger::debug("Nick: Invalid username. Command ignored");
		return SUCCESS;
	}
	std::string host = Parser::getParam(params, 3);
	if (Parser::validHostname(host) == 0)
	{
		Logger::debug("Nick: Invalid hostname. Command ignored");
	}
	std::string serverToken = Parser::getParam(params, 4);
	//TODO check if valid severToken ?
	std::string umode = Parser::getParam(params, 5);
	(void) umode;
	//TODO do method that transform umode in form "+iw" to uint
	std::string realname = Parser::getParam(params, 6);
	RelayedClient *rClient = new RelayedClient(this->getServer(), executor, hopcount, nick, username, realname, 0, serverToken, host );
	this->getServer()._addClient(*rClient, NULL);
	Logger::info("new relayed client registered: " + rClient->getName() + " (" + rClient->getUID() + "!" + rClient->getName() + "@"
													 + rClient->getHostname() + " is " + ntos(hopcount) + " hop(s) away)");
	//Forward and backward this info
	std::stringstream reply_msg;
	reply_msg << "NICK " << rClient->getUID() << " " << rClient->getHopCount() + 1 << " " << rClient->getName() << " " << rClient->getHostname() << " " << rClient->getServerToken() << " " << rClient->getModeString() << " :" << rClient->getRealname();
	Logger::debug("sending to all servers except: " + executor.getUID());
	this->getServer()._sendAllServers(reply_msg.str(), &executor);
	return SUCCESS;
}

bool				CommandNick::hasPermissions(AEntity & executor)
{
	( void) executor;
	return true;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */