#include "CommandSquit.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandSquit::CommandSquit(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandSquit::~CommandSquit()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

uint					CommandSquit::operator()(NetworkEntity & executor, std::string params)
{
	if (Parser::nbParam(params) < 2)
	{
		this->getServer()._sendMessage(executor, this->getServer().makePrefix(NULL, NULL) + ERR_NEEDMOREPARAMS(executor.getUID(), std::string("SQUIT")));
		return SUCCESS;
	}
	std::string serverToken = Parser::getParam(params, 0);
	std::string comment = Parser::getParam(params, 1);
	if (executor.getType() & Client::value_type)
	{
		//Squit from a client
		if (static_cast<Client &>(executor).isServerOP() == false)
		{
			this->getServer()._sendMessage(executor, this->getServer().makePrefix(NULL, NULL) + ERR_NOPRIVILEGES(executor.getUID()));
			return SUCCESS;
		}
		return this->_quitServer(executor, serverToken, comment);
	}
	else
	{
		Logger::debug("SQUIT command from server");
		const AEntity* emitter = this->getClient();
		//TODO reform here when prefix parsing system has been reviewed
		if (emitter == NULL)
		{
			Logger::critical("Invalid prefix: no emitter transmitted");
			return SUCCESS;
		}
		else if (emitter->getFamily() == SERVER_ENTITY_FAMILY)
		{
			if (this->getServer()._servers.count(serverToken) == 0)
			{
				this->getServer()._sendMessage(executor, this->getServer().makePrefix(NULL, NULL) + ERR_NOSUCHSERVER(executor.getUID(), serverToken));
				return SUCCESS;
			}
			//if a server send a SQUIT emitted by a server it's indicate a remote server link has been dropped, handle this update and send update to all others
			Logger::info("Server " + serverToken + " had leave the network cause: " + comment);
			Logger::critical("Handle a pseudo-netsplit here");
			this->getServer()._sendAllServers(
				":" + emitter->getUID() + " SQUIT " + serverToken + " :" + comment,
				&executor
			);
		}
		else
		{
			//if a server send a SQUIT emitted by a client it's indicate that a server link should be dropped, handle it or forward it
			return this->_quitServer(*emitter, serverToken, comment);
		}	

	}
	return SUCCESS;
}

uint				CommandSquit::_quitServer(const AEntity & emitter, std::string & serverToken, std::string & comment)
{

	if (this->getServer()._servers.count(serverToken) == 0)
	{
		this->getServer()._sendMessage(const_cast<AEntity &>(emitter), this->getServer().makePrefix(NULL, NULL) + ERR_NOSUCHSERVER(emitter.getUID(), serverToken));
		return SUCCESS;
	}
	AEntity *server = this->getServer()._servers[serverToken];
	if (server->getType() & RelayedServer::value_type)
	{
		//if a client ask to drop link with a remote server link, just forward this request
		this->getServer()._sendMessage(*server,
			":" + emitter.getUID() + " SQUIT " + serverToken + " :" + comment
		);
	}
	else
	{
		// a client request to drop link with a local server link, drop it and send an update to all others
		Logger::warning("Drop local server link with: " + serverToken + " with cause of: " + comment);
		Logger::critical("Handle a netsplit here");
		this->getServer().disconnect(reinterpret_cast<Server*>(server)->getStream());
		this->getServer()._sendAllServers(
			":" + this->getServer().getUID() + " SQUIT " + serverToken + " :" + comment
		);
	}
	return SUCCESS;
}

bool				CommandSquit::hasPermissions(AEntity & executor)
{
	//TODO implement right for SQUIT
	if (executor.getType() & UnRegisteredConnection::value_type)
		return false;
	return true;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */