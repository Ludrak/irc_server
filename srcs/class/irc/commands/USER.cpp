# include "IRCServer.hpp"

UnRegiste
static uint					_commandUSERserver(AEntity & executor, std::string params)
{
	if (Parser::nbParam(params) < 4)
		return this->_reply(executor, ERR_NEEDMOREPARAMS, "USER");
	std::string target = Parser::getParam();
	if ()
	this->getUnRegisteredConnectionByUID();
	executor.setUsername(Parser::getParam(params, 0));
	executor.setDomaineName(Parser::getParam(params, 1));
	executor.setServername(Parser::getParam(params, 2));
	executor.setRealname(Parser::getParam(params, 3));
	this->_setRegistered(executor, Client::value_type_client);
	Logger::info("new user registered: " + executor.getNickname());
}

static uint					_commandUSERunknown(UnRegisteredConnection * executor, std::string params)
{
	if (Parser::nbParam(params) < 4)
		return this->_sendMessage(executor, ERR_NEEDMOREPARAMS("USER"));
	if (executor.getNickname().empty() == false)
		return SUCCESS;
	Client *new_client = this->_registerClient(executor, Client::value_type);;
	if (new_client == NULL)
		return -1;
	//TODO set here param syntax validation
	if (Parser::validUser(Parser::getParam(params, 0)))
	{
		new_client->setUsername(Parser::getParam(params, 0));
		new_client->setDomaineName(Parser::getParam(params, 1));
		// Unused
		// new_client->setServername(Parser::getParam(params, 2));
		new_client->setRealname(Parser::getParam(params, 3));
		Logger::info("new user registered: " + new_client.getNickname());
	}
	return SUCCESS;
}

uint						IRCServer::_commandUSER(AEntity & executor, std::string params)
{
	//TOFIX: can register without nickname
	Logger::debug("<" + ntos(executor.getStream().getSocket()) + "> Command<USER> with args: " + params );
	if (executor.getFamilly() == CLIENT_ENTITY_FAMILY)
			return this->_sendMessage(executor, ERR_ALREADYREGISTRER("USER"));
	else if (executor.getFamilly() == SERVER_ENTITY_FAMILY)
		_commandUSERserver(reinterpret_cast<UnRegisteredConnection*>(&executor));
	else if (executor.getType() & UnregisteredConnection::value_type)
		_commandUSERunknown(executor);
	else
	{
		Logger::critical("Error: Invalid AEntity familly/type: " + ntos(executor.getFamily()) + "/" + ntos(executor.getType()));
		return -1;
	}
	return SUCCESS;
}