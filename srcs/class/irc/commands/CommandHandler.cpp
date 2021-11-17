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

uint			CommandHandler::handle(AEntity & executor, std::string data)
{
	Server			*sender = NULL;
	AEntity			*client = NULL;
	AEntity			*clientHost = NULL;
	if (data[0] == ':')
	{
		this->_server.parsePrefix(data.substr(0, data.find(" ")), &sender, reinterpret_cast<RelayedClient**>(&client), reinterpret_cast<RelayedServer**>(&clientHost));
		data = data.substr(data.find(" ") + 1, data.size() - data.find(" ") - 1);
		if (sender == NULL)
			return (1);
	}
	std::string command = data.substr(0, data.find(" "));
	if (this->_commands.count(command) == 1)
	{
		ACommand & cmd = *this->_commands[command];
		if ( cmd.hasPermissions(executor))
		{
			cmd.setSender(sender);
			cmd.setClient(client);
			cmd.setClientHost(clientHost);
			return cmd(executor, data.substr(command.size() + 1, data.size() - (command.size() + 1)));
		}
		else if (client)
		{
			this->_server._sendMessage(reinterpret_cast<Client*>(client)->getStream(), ERR_UNKNOWNCOMMAND(data));
			return SUCCESS; 
		}

	}
	// else if (executor.getType() & Client::value_type || executor.getType() & Server::value_type)
	// 	this->_server._sendMessage(reinterpret_cast<NetworkEntity*>(executor)->getStream(), ERR_UNKNOWNCOMMAND(data));
	// else if (executor.getType() & RelayedClient::value_type || executor.getType() & RelayedServer::value_type)
	// 	this->_server._sendMessage(reinterpret_cast<RelayedEntity*>(executor)->getServer().getStream(), ERR_UNKNOWNCOMMAND(data));
	else
	{
		Logger::critical(std::string("unhandled AEntity type type: ") + ntos(executor.getType()));
		return 1;
	}
	return SUCCESS;
}
	// if (e->getType() & Client::value_type || client)
	// {
	// 	/* no prefix defined so we use executor */
	// 	if (!client)
	// 		client = reinterpret_cast<Client*>(e);
		
	// 	/* check if command is part of list of operations */
	// 	if (this->_commands.count(command) == 0)
	// 	{
	// 		if (client->getType() & Client::value_type)
	// 		{
	// 			Logger::debug("user sent command not found (" + client->getUID() + "@" + reinterpret_cast<Client*>(client)->getStream().getIP() + ")");
	// 			this->_sendMessage(reinterpret_cast<Client*>(client)->getStream(), ERR_UNKNOWNCOMMAND(data));
	// 		}
	// 		else if (client->getType() & RelayedClient::value_type)
	// 		{
	// 			Logger::debug("relayed user sent unknown command (" + client->getUID() + "@" + reinterpret_cast<RelayedClient*>(client)->getServer().getStream().getIP() + ")");
	// 			this->_sendMessage(reinterpret_cast<RelayedClient*>(client)->getServer().getStream(), ERR_UNKNOWNCOMMAND(data));
	// 		}
	// 		return (1);
	// 	}
	// 	/* execute command from command list */
	// 	return ((this->*(_userCommands[command]))(*client, data.substr(command.size() + 1, data.size() - (command.size() + 1))));
	// }
	// else if ((e->getFamily() & SERVER_ENTITY_FAMILY) && sender)
	// {
	// 	/* check if command is part of list of operations */
	// 	if (this->_serverCommands.count(command) == 0)
	// 	{
	// 		if (e->getType() & Server::value_type)
	// 		{
	// 			Logger::debug("server sent unknown command (" + e->getUID() + "@" + reinterpret_cast<Server*>(client)->getStream().getIP() + ")");
	// 			this->_sendMessage(reinterpret_cast<Server*>(client)->getStream(), ERR_UNKNOWNCOMMAND(data));
	// 		}
	// 		else if (client->getType() == RelayedServer::value_type)
	// 		{
	// 			Logger::debug("relayed server sent unknown command  (" + client->getUID() + "@" + reinterpret_cast<RelayedServer*>(client)->getServer().getStream().getIP() + ")");
	// 			this->_sendMessage(reinterpret_cast<RelayedServer*>(client)->getServer().getStream(), ERR_UNKNOWNCOMMAND(data));
	// 		}
	// 		return (1);
	// 	}
	// 	/* execute command from command list */
	// 	return ((this->*(_serverCommands[command]))(*client, data.substr(command.size() + 1, data.size() - (command.size() + 1))));
	// }
	// // TODO ADD UNREGISTERED                                
	// // TODO find a way to fit all 3 client types in one call
	// Logger::critical("Command '" + command + "' has no viable executor");
	// return (1);}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */