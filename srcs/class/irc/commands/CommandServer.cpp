#include "CommandServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandServer::CommandServer(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandServer::~CommandServer()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
   Command: SERVER
   Parameters: <servname> <hopcount> <token> [:<info>]
*/

# include <iomanip>
uint				CommandServer::operator()(NetworkEntity & executor, std::string params)
{
	uint nParams = Parser::nbParam(params);
	if (nParams != 3 && nParams != 4)
		return SUCCESS;

	std::string servname = Parser::getParam(params, 0);
	int			hopcount;
	int			token;
	std::string info = Parser::getParam(params, 3);
	try 
	{ hopcount = std::stoi(Parser::getParam(params, 1)); }
	catch (std::invalid_argument e)
	{ Logger::warning("Server registered with an invalid hopcount: " + Parser::getParam(params, 1)); }

	try 
	{ token = std::stoi(Parser::getParam(params, 2)); }
	catch (std::invalid_argument e)
	{ Logger::warning("Server registered with an invalid token: " + Parser::getParam(params, 2)); }
	
	std::stringstream tokss;
	tokss << std::setfill('0') << std::setw(3) << token;
	std::stringstream ss;
	ss << "SERVER " << servname << " " << (hopcount + 1) << " " << tokss.str() << " :" << info;
	
	if  (this->getServer()._servers.count(tokss.str()) || tokss.str() == this->getServer().getUID())
	{
		// TODO prefix
		Package *explosive = new Package(this->getServer().getProtocol(),
			this->getServer().getProtocol().format(ERR_ALREADYREGISTRED()), &executor.getStream(), true);
		Logger::warning("Send explosive package to executor");
		this->getServer().sendPackage(explosive, executor.getStream());
		return SUCCESS;
	}
	if (executor.getType() & UnRegisteredConnection::value_type)
	{
		// new server joined the our network
		int type = (executor.getStream().getType() == CLIENT) ? Server::value_type_forward : Server::value_type;
		Server *new_serv = new Server(static_cast<UnRegisteredConnection&>(executor), tokss.str(),
			servname, info, executor.getStream().getHost(), type);
		this->getServer()._sendAllServers(ss.str(), new_serv);
		std::stringstream reply_msg;
		reply_msg << "SERVER " << this->getServer().getName() << " 0 " << this->getServer().getUID() << " :" << this->getServer().getInfo();
		Logger::debug("Resend (server infos): " + reply_msg.str());
		if (type & Server::value_type)
		{
			this->getServer()._sendMessage(*new_serv, reply_msg);
			for (std::map<std::string, AEntity*>::iterator it = this->getServer()._servers.begin();
			it != this->getServer()._servers.end();
			++it)
			{
				if (it->second->getUID() == new_serv->getUID()
				|| it->second->getUID() == this->getServer().getUID())
					continue ;
				reply_msg.str("");
				if (it->second->getType() & (Server::value_type | Server::value_type_forward))
				{
					Server *server = reinterpret_cast<Server*>(it->second);
					reply_msg << "SERVER " << server->getName() << " 1 " << server->getUID() << " :" << server->getInfo();
				}
				else if (it->second->getType() & RelayedServer::value_type)
				{
					RelayedServer *server = reinterpret_cast<RelayedServer*>(it->second);
					reply_msg << "SERVER " << server->getName() << " " << server->getHopCount() + 1 << " " << server->getUID() << " :" << server->getInfo();
				}
				else 
					continue;
				this->getServer()._sendMessage(*new_serv, reply_msg);
			}
		}
		else
			Logger::info("Connection to forward success");
		this->getServer()._addServer(*new_serv, reinterpret_cast<UnRegisteredConnection*>(&executor));
		// TODO send all users w/ NICK (extended)
		// TODO send all channels w/ NJOIN
		Logger::info ("new server joined the network (" + new_serv->getUID() + "@" + new_serv->getStream().getHost() + ")");
	}
	else if ((executor.getType() & (Server::value_type | Server::value_type_forward)) && hopcount > 0)
	{
		// server is introducing a new relayed server
		Logger::debug("server introducting a new relayed");
		std::stringstream tok;
		tok << std::setfill('0') << std::setw(3) << token;
		RelayedServer *new_serv = new RelayedServer(static_cast<Server&>(executor), hopcount, tok.str(),
			servname, info, executor.getStream().getHost());
		this->getServer()._addServer(*new_serv, NULL);
		Logger::debug("Resend: " + ss.str());
		this->getServer()._sendAllServers(ss.str(), &executor);
		Logger::info ("new server joined the network (" + new_serv->getUID() + "@" + new_serv->getHostname() + " is " + ntos(new_serv->getHopCount()) + " hop(s) away)");
	}
	else if (Server::value_type_forward && hopcount == 0)
	{
		Logger::debug("value_type_forward hopcount == 0");
		return SUCCESS;
	}
	else
	{
		//TODO kick
		Package *explosive = new Package(this->getServer().getProtocol(),
			this->getServer().getProtocol().format("ERROR :invalid parameters"), &executor.getStream(), true);
		Logger::warning("Send explosive package to executor");
		this->getServer().sendPackage(explosive, executor.getStream());
		return SUCCESS;
	}
	Logger::debug("out of function commandServer");
	return SUCCESS;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandServer::hasPermissions(AEntity & executor)
{
	if (executor.getType() & UnRegisteredConnection::value_type)
		return true;
	if (executor.getType() & Server::value_type)
		return true;
	if (executor.getType() & Server::value_type_forward)
		return true;
	if (executor.getType() & (UnRegisteredConnection::value_type | Server::value_type | Server::value_type_forward))
		return (true);
	return (false);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */