#include "CommandHandler.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandHandler::CommandHandler( IRCServer & server) : _server(server), _connectionEmitter(NULL)
{
}

CommandHandler::CommandHandler( const CommandHandler & src ) :  _server(src._server), _connectionEmitter(NULL), _commands(src._commands)
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandHandler::~CommandHandler()
{
	for (std::map<std::string, ACommand*>::iterator it = this->_commands.begin(); it != this->_commands.end(); ++it)
	{
		delete it->second;
	}
	this->_commands.clear();
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

CommandHandler &				CommandHandler::operator=( CommandHandler const & rhs )
{
	if ( this != &rhs )
	{
		this->_commands = rhs._commands;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, CommandHandler const & i )
{
	//REVIEW 
	( void ) i;
	o << "Handler = TODO";
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

uint			CommandHandler::handle(NetworkEntity & executor, std::string & data)
{
	// Server			*sender = NULL;
	AEntity			*emitter = NULL;
	RelayedServer	*clientHost = NULL;
	std::string		uname;

	Logger::debug("Raw message: " + data);
	if (data[0] == ':')
	{
		/* now, emitter is always filled with valid data */
		this->_server.parsePrefix(executor, data.substr(0, data.find(" ")), &clientHost, &emitter, &uname);
		data = data.substr(data.find(" ") + 1, data.size() - data.find(" ") - 1);
		data = data.substr(data.find_first_not_of(" "), data.size() - data.find_first_not_of(" "));
		if (clientHost == NULL)
		{
			//REVIEW In the end remove this debug
			Logger::debug("handler: clientHost is NULL");
		}
		if (emitter == NULL)
		{
			Logger::critical("Handler: Emitter is NULL");
			return ERROR;
		}
	}
	else
	{
		//TODO change all needed things to allow executing parsePrefix in all cases (even when no prefix are given)
		if (emitter == NULL)
			emitter = &executor;
	}
	size_t cmdEnd = data.find(" ");
	std::string command = data.substr(0, cmdEnd);
	if (cmdEnd == std::string::npos)
		cmdEnd = data.size();
	else
		cmdEnd += 1;
	std::istringstream is (command);
	int err = -1;
	is >> err;
	if (err > 0)
		return this->numericReplyReceived(err, *emitter, data);
	else if (this->_commands.count(command) == 1)
	{
		ACommand & cmd = *(this->_commands[command]);
		if ( cmd.hasPermissions(executor))
		{
			/* Permission allowed */
			// cmd.setSender(sender);
			cmd.setEmitter(*emitter);
			cmd.setClientHost(clientHost);
			Logger::debug("command " + command + " (" + executor.getUID() + "@" + executor.getStream().getHost() + ")");
			std::string params = data.substr(cmdEnd, data.size() - command.size());
			Logger::debug("params: |" + params + "|");
			return cmd(executor, params);
		}
		else
			Logger::warning("Not enought privilegies (" + command + ")");
	}
	else
		Logger::warning("Unknown command (" + command + ")");
	this->_server._sendMessage(*emitter, ERR_UNKNOWNCOMMAND(emitter->getUID(), command));
	return SUCCESS;
}



uint		CommandHandler::numericReplyReceived(int error, AEntity & emitter, std::string & data)
{
	if (error >= 400)
		Logger::debug("ERR (" + ntos(error) + "): " + data);
	else
		Logger::debug("RPL (" + ntos(error) + "): " + data);
	std::string targetName = Parser::getParam(data, 1);
	if (targetName == this->_server.getUID())
	{
		if (error == 401)
		{
			Logger::debug("Receiving `No such nick or channel` is normal after a nick collision");
			return SUCCESS;
		}
		else if (error == 462)
		{
			if (this->getConnectionEmitter() != NULL)
				return this->transmitingReply(*this->getConnectionEmitter(), data);
		}
		Logger::error("Receiving an unhandled numeric reply: " + data);
		return SUCCESS;
	}
	else if (this->_server._entities.count(targetName) == 0)
	{
		Logger::warning("Handler: Invalid target: " + targetName);
		return SUCCESS;
	}
	else
		return this->transmitingReply(targetName, emitter, data);
	return SUCCESS;
}

uint		CommandHandler::transmitingReply(std::string & targetName, AEntity & emitter, std::string & data)
{
	Logger::info("Transmiting numeric response to: " + targetName);
	AEntity *target = this->_server._entities[targetName];
	this->_server._sendMessage(*target, emitter.getPrefix() + data);
	return SUCCESS;
}

uint		CommandHandler::transmitingReply(AEntity & target, std::string & data)
{
	Logger::info("Transmiting numeric response to: " + target.getUID());
	this->_server._sendMessage(target, data);
	return SUCCESS;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

IRCServer&				CommandHandler::getServer( void )
{
	return this->_server;
}

AEntity*				CommandHandler::getConnectionEmitter( void ) const
{
	return this->_connectionEmitter;
}

void					CommandHandler::setConnectionEmitter(AEntity & emitter)
{
	this->_connectionEmitter = &emitter;
}

void					CommandHandler::unsetConnectionEmitter( void )
{
	this->_connectionEmitter = NULL;
}


ACommand*				CommandHandler::getCommand(std::string & command_name)
{
	if (this->_commands.count(command_name) == 1)
		return this->_commands[command_name];
	else
		return NULL;
}

/* ************************************************************************** */