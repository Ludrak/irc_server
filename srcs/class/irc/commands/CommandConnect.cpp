#include "CommandConnect.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandConnect::CommandConnect(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandConnect::~CommandConnect()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/
//TODO sendPackage is not _sendPackage

/*
	Command: CONNECT
	Parameters: <target server> <port> [ <remote server> ]
*/
uint					CommandConnect::operator()(NetworkEntity & executor, std::string params)
{
	if (executor.getType() & Client::value_type)
	{
		Logger::debug("Connect: executor is a client");
		Client& client = static_cast<Client&>(executor);
		if (client.isServerOP() == false)
		{
			Logger::debug("Connect: executor isn't OP");
			this->getServer()._sendMessage(executor, executor.getPrefix() + ERR_NOPRIVILEGES(executor.getUID()));
			return SUCCESS;
		}
	}
	uint nbParam = Parser::nbParam(params);
	if (nbParam < 2)
	{
		this->getServer()._sendMessage(executor, ERR_NEEDMOREPARAMS(executor.getUID(), std::string("CONNECT")));
		return SUCCESS;
	}
	std::string target = Parser::getParam(params, 0);
	std::string port_string = Parser::getParam(params, 1);
	ushort port = 0;
	try {
		std::istringstream is(port_string);
		is >> port;
	}
	catch (const std::invalid_argument & e)
	{	
		Logger::debug("Connect: Invalid port number received");
		return SUCCESS;
	}
	if (nbParam > 2)
	{
		/* a remote server is setted */
		/* forward it the request */
		std::string forward = Parser::getParam(params, 2); 
		return this->_forwardRequest(executor, target, port_string, forward);
	}
	/* execute CONNNECT command */
	/* try to connect to target server*/
	Logger::info("Try connecting to server: " + target);
	try {
		this->getServer().connectOn(target, port, this->getServer().getProtocol(), this->getServer()._useTLS);
	}
	catch (const AClient::ConnectionException & e )
	{
		Logger::warning("Connection to server: " + target + " failed");
		//TODO do a WALLOPS here?
		return SUCCESS;
	}
	//TODO do a WALLOPS here
	return SUCCESS;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

uint				CommandConnect::_forwardRequest(NetworkEntity & executor, std::string & target, std::string & port, std::string & forward)
{
	Logger::debug("Forward CONNECT request" + target);

	if (this->getServer()._servers.count(forward) == 0)
	{
		/* unknown remote server */
		this->getServer()._sendMessage(
			executor, ":" + this->getServer().getUID() + " " + 
			ERR_NOSUCHSERVER(executor.getUID(), forward));
		return SUCCESS;
	}

	AEntity&	emitter = this->getEmitter();
	AEntity*	remoteServer = this->getServer()._servers[forward];
	if (remoteServer->getType() & RelayedServer::value_type)
		this->getServer()._sendMessage(*remoteServer, emitter.getPrefix() + " CONNECT " + target + " " + port + " " + forward);
	else
		this->getServer()._sendMessage(*remoteServer, emitter.getPrefix() + " CONNECT " + target + " " + port);
	return SUCCESS;
}

bool				CommandConnect::hasPermissions(AEntity & executor)
{
	// if (executor.getType() & Client::value_type)
		// return true;
	if (executor.getType() & UnRegisteredConnection::value_type)
		return false;
	return true;
}

/* ************************************************************************** */