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
	(void) executor;
	AEntity &emitter = this->getEmitter();
	if (emitter.getFamily() == SERVER_ENTITY_FAMILY)
	{
		Logger::error("MOTD: Command requested by a server");
		return SUCCESS;
	}
	else if ((emitter.getType() & (Client::value_type | RelayedClient::value_type)) == false)
	{
		Logger::critical("MOTD: unsupported type given: " + ntos(emitter.getType()));
		return ERROR;
	}
	uint		nbParam = Parser::nbParam(params);
	if (nbParam > 0)
	{
		/* Forward the request to concerned server */
		std::string serverId = Parser::getParam(params, 0);
		if (this->getServer()._servers.count(serverId) == 0)
		{
			Logger::debug("MOTD: Invalid target given");
			this->getServer()._sendMessage(emitter, ERR_NOSUCHSERVER(emitter.getUID(), serverId));
			return SUCCESS;
		}
		AEntity *fwdServ = this->getServer()._servers[serverId];
		if (fwdServ == NULL)
		{
			Logger::critical("MOTD: fwdServ should never be NULL");
			return ERROR;
		}
		this->getServer()._sendMessage(*fwdServ, emitter.getPrefix() + " MOTD " + serverId);
		return SUCCESS;
	}
	/* Send motd to emitter of the request */
	char		buffer[81];
	std::string	path;
	bool		isServerOP = false;
	if (emitter.getType() & RelayedClient::value_type)
		isServerOP = static_cast<RelayedClient&>(emitter).isServerOP();
	else
		isServerOP = static_cast<Client&>(emitter).isServerOP();
	if (isServerOP)
		path = this->getServer().getMotdsPath() + "/oper.motd";
	else if (this->_shortEnabled && this->getServer()._shortMotdEnabled)
		path = this->getServer().getMotdsPath() + "/ircd.smotd";
	else
		path = this->getServer().getMotdsPath() + "/ircd.motd";
	Logger::debug("MOTD: Use file: " + path);
	std::ifstream	mFile(path.c_str());
	if (!mFile.is_open() || !Parser::isRegularFile(path.c_str()))
	{
		Logger::warning("MOTD: file not found: " + path);
		this->getServer()._sendMessage(emitter,
			this->getServer().getPrefix() + ERR_NOMOTD(emitter.getUID()));
		return SUCCESS;
	}
	this->getServer()._sendMessage(emitter,
		this->getServer().getPrefix() +
		RPL_MOTDSTART(emitter.getUID(), this->getServer().getUID()));
	while (!mFile.eof())
	{
		mFile.getline(buffer, 80);
		buffer[mFile.gcount()] = '\0';
		this->getServer()._sendMessage(emitter,
			this->getServer().getPrefix() + RPL_MOTD(emitter.getUID(), buffer));
	}
	this->getServer()._sendMessage(emitter,
		this->getServer().getPrefix() + RPL_ENDOFMOTD(emitter.getUID()));

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