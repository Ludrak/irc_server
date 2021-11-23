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
		//TODO check and handle emitter == server
		char buffer[81];
		//TODO handle directory reading
		std::string filename;
		if (reinterpret_cast<Client*>(emitter)->isServerOP())
			filename = "/oper.motd";
		else if (this->_shortEnabled && this->getServer()._shortMotdEnabled)
			filename = "/ircd.smotd";
		else
			filename = "/ircd.motd";
		std::ifstream	mFile(this->getServer().getMotdsPath() + filename);
		Logger::debug("Use motd file: " + this->getServer().getMotdsPath() + filename);
		if (!mFile.is_open())
		{
			this->getServer()._sendMessage(*emitter,
				":" + emitter->getUID() + "@" + reinterpret_cast<Client *>(emitter)->getHostname()	+ " " +
				ERR_NOMOTD(emitter->getUID()));
			return SUCCESS;
		}
		this->getServer()._sendMessage(*emitter,
			":" + emitter->getUID() + "@" + reinterpret_cast<Client *>(emitter)->getHostname() + " " +
			RPL_MOTDSTART(emitter->getUID(), this->getServer().getUID()));
		std::string line;
		while (!mFile.eof())
		{
			mFile.getline(buffer, 80);
			buffer[mFile.gcount()] = '\0';
			line = buffer;
			this->getServer()._sendMessage(*emitter,
				":" + emitter->getUID() + "@" + reinterpret_cast<Client *>(emitter)->getHostname() + " " +
				RPL_MOTD(emitter->getUID(), line));
		}
		this->getServer()._sendMessage(*emitter,
			":" + emitter->getUID() + "@" + reinterpret_cast<Client *>(emitter)->getHostname() + " " +
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
			this->getServer()._sendMessage(*target, ":" + emitter->getUID() + "@" + reinterpret_cast<Client *>(emitter)->getHostname() + " MOTD " + targetName);
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


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */