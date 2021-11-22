#include "CommandHandler.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandHandler::CommandHandler( IRCServer & server) : _server(server)
{
}

CommandHandler::CommandHandler( const CommandHandler & src ) :  _server(src._server), _commands(src._commands)
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

uint			CommandHandler::handle(NetworkEntity & executor, std::string data)
{
	// Server			*sender = NULL;
	RelayedClient	*client = NULL;
	RelayedServer	*clientHost = NULL;
	std::string		uname;

	Logger::debug("Raw message: " + data);
	if (data[0] == ':')
	{
		this->_server.parsePrefix(data.substr(0, data.find(" ")), &clientHost, &client, &uname);
		data = data.substr(data.find(" ") + 1, data.size() - data.find(" ") - 1);
		data = data.substr(data.find_first_not_of(" "), data.size() - data.find_first_not_of(" "));
		if (clientHost == NULL)
		{
			Logger::debug("handler: clientHost is NULL");
			return (1);
		}
	}
	Logger::debug("Is it a usefull message ? - handling command: " + data);
	std::string command = data.substr(0, data.find(" "));
	try {
		int err = std::stoi(command);
		if (err >= 400)
			Logger::error("ERR: " + data);
		else
			Logger::info("RPL: " + data);

	} catch(std::invalid_argument &e)
	{
		if (this->_commands.count(command) == 1)
		{
			ACommand & cmd = *(this->_commands[command]);
			if ( cmd.hasPermissions(executor))
			{
				// cmd.setSender(sender);
				cmd.setClient(client);
				cmd.setClientHost(clientHost);
				Logger::debug("command " + command + " (" + executor.getStream().getIP() + ")");
				return cmd(executor, data.substr(command.size() + 1, data.size() - (command.size() + 1)));
			}
			else
			{
				Logger::warning("Not enought privilegies (" + command + ")");
				this->_server._sendMessage(executor, ERR_UNKNOWNCOMMAND(executor.getUID(), command));
			}
		}
		else {
			this->_server._sendMessage(executor, ERR_UNKNOWNCOMMAND(executor.getUID(), command));
			Logger::warning("Unknown command (" + command + ")");
		}
	}
	return SUCCESS;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

IRCServer&				CommandHandler::getServer( void )
{
	return this->_server;
}

/* ************************************************************************** */