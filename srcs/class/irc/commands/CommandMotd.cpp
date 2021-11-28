#include "CommandMotd.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandMotd::CommandMotd(CommandHandler & handler) : ACommand(handler), _shortEnabled(false)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandMotd::~CommandMotd()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
	Command: MOTD
	Parameters: [ <target> ]
*/
uint					CommandMotd::operator()(NetworkEntity & executor, std::string params)
{
	AEntity *emitter;
	if (executor.getFamily() == SERVER_ENTITY_FAMILY)
	{
		emitter = const_cast<AEntity*>(this->getClient());
		if (emitter == NULL)
		{
			Logger::error("MOTD: Receive an invalid prefix");
			return SUCCESS;
		}
	}
	else
		emitter = &executor;
	uint nbParam = Parser::nbParam(params);
	if (nbParam == 0)
	{
		//TODO check and handle emitter == a server
		//TODO check and handle emitter == a relayedClient (cast error)
		//TODO add clean prefixs
		char buffer[81];
		std::string filename;
		if (reinterpret_cast<Client*>(emitter)->isServerOP())
			filename = "/oper.motd";
		else if (this->_shortEnabled && this->getServer()._shortMotdEnabled)
			filename = "/ircd.smotd";
		else
			filename = "/ircd.motd";
		std::string path = this->getServer().getMotdsPath() + filename;
		std::ifstream	mFile(path.c_str());
		Logger::debug("Use motd file: " + path);
		if (!mFile.is_open() || !Parser::isRegularFile(path.c_str()))
		{
			Logger::warning("MOTD: file not found: " + path);
			this->getServer()._sendMessage(*emitter,
				reinterpret_cast<Client *>(emitter)->getPrefix() + ERR_NOMOTD(emitter->getUID()));
			return SUCCESS;
		}
		this->getServer()._sendMessage(*emitter,
			reinterpret_cast<Client *>(emitter)->getPrefix() +
			RPL_MOTDSTART(emitter->getUID(), this->getServer().getUID()));
		std::string line;
		while (!mFile.eof())
		{
			mFile.getline(buffer, 80);
			buffer[mFile.gcount()] = '\0';
			line = buffer;
			this->getServer()._sendMessage(*emitter,
				reinterpret_cast<Client *>(emitter)->getPrefix() + RPL_MOTD(emitter->getUID(), line));
		}
		this->getServer()._sendMessage(*emitter,
			reinterpret_cast<Client *>(emitter)->getPrefix() +
			RPL_ENDOFMOTD(emitter->getUID()));
	}
	else if (nbParam == 1)
	{
		std::string targetName = Parser::getParam(params, 0);
		if (this->getServer()._servers.count(targetName) == 0)
		{
			Logger::debug("Invalid target given");
			return SUCCESS;
		}
		AEntity *target = this->getServer()._servers[targetName];
		if (!(target->getType() & RelayedServer::value_type))
			targetName = "";
		//TODO put prefix
		if (emitter->getType() & Client::value_type)
			this->getServer()._sendMessage(*target,reinterpret_cast<Client *>(emitter)->getPrefix() + " MOTD " + targetName);
		else if (emitter->getType() & Server::value_type)
			this->getServer()._sendMessage(*target, ":" + emitter->getUID() + "@" + reinterpret_cast<Server *>(emitter)->getHostname() + " MOTD " + targetName);
		else if (emitter->getType() & RelayedServer::value_type)
			this->getServer()._sendMessage(*target, ":" + emitter->getUID() + "@" + reinterpret_cast<RelayedServer *>(emitter)->getHostname() + " MOTD " + targetName);
		else
			Logger::critical("MOTD: unhandled type");
	}
	return SUCCESS;
}

uint				CommandMotd::operator()(NetworkEntity & executor, std::string params, bool useShort)
{
	this->_shortEnabled = useShort;
	uint ret = this->operator()(executor, params);
	this->_shortEnabled = false;
	return ret;
}

bool				CommandMotd::hasPermissions(AEntity & executor)
{
	if (executor.getType() & UnRegisteredConnection::value_type)
		return false;
	return true;
}

/* ************************************************************************** */