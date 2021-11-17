#include "CommandPrivmsg.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandPrivmsg::CommandPrivmsg(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandPrivmsg::~CommandPrivmsg()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

uint					CommandPrivmsg::operator()(NetworkEntity & executor, std::string params)
{
	switch (Parser::nbParam(params))
	{
		case 0:
			this->getServer()._sendMessage(executor, ERR_NORECIPIENT(params));
			break ;
		case 1:
			this->getServer()._sendMessage(executor, ERR_NOTEXTTOSEND());
			break ;
		case 2:
			//* right numbers of params */
			std::string targetName = Parser::getParam(params, 0);
			Logger::debug("Privmsg target: " + targetName); 
			if (this->getServer()._entities.count(targetName) == 0)
			{
				Logger::debug("Privmsg: Target not found"); 
				this->getServer()._sendMessage(executor, ERR_NOSUCHNICK(targetName));
				return SUCCESS;
			}
			AEntity *target = this->getServer()._entities[targetName];
			if (target->getType() & Channel::value_type)
			{
				Logger::debug("Privmsg: target is a channel");
				if (reinterpret_cast<Channel *>(target)->isRegistered(static_cast<Client&>(executor)) == false)
				{
					Logger::debug("Privmsg: not in channel");
					this->getServer()._sendMessage(executor.getStream(), ERR_CANNOTSENDTOCHAN(target->getUID()));
					return SUCCESS;
				}
			}
			Logger::info(executor.getUID() + " send message to " + target->getUID());
			std::string msg = Parser::getParam(params, 1);
			msg = this->getServer().makePrefix(&executor, &this->getServer()) + "PRIVMSG " + target->getUID() + " :" + msg;
			Logger::debug(std::string("MSG = ") + msg);
			this->getServer()._sendMessage(*target, msg);
			break ;
	}
	return SUCCESS;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandPrivmsg::hasPermissions(AEntity & executor)
{
	//TODO implement right for Privmsg
	// Do we steel receive from SERVER?
	// isn't it only by Client or unregistered client?
	// if (executor.getType() & UnRegisteredConnection::value_type)
		// return false;
	if (executor.getFamily() != CLIENT_ENTITY_FAMILY)
		return false;
	return true;
}

// TODO to delete
// uint				CommandPrivmsg::_CommandPrivmsgunknown(UnRegisteredConnection & executor, std::string params)
// {
// 	if (Parser::nbParam(params) < 4)
// 	{
// 		this->getServer()._sendMessage(executor, ERR_NEEDMOREPARAMS(std::string("USER")));
// 		return SUCCESS;
// 	}
// 	else if (executor.getUID().empty() == true)
// 		return SUCCESS;
// 	std::string username = Parser::getParam(params, 0);
// 	//REVIEW set more params syntax check
// 	if (Parser::validUser(username) == false)
// 		return SUCCESS;
// 	//REVIEW catch throw in stoi
// 	Client *new_client = new Client(this->getServer(),
// 								executor,
// 								std::stoi(Parser::getParam(params, 1)),
// 								Parser::getParam(params, 3));
// 	new_client->setName(username);
// 	this->getServer()._addClient(*new_client, &executor);
// 	Logger::info("new user registered: " + new_client->getUID());
// 	return SUCCESS;
// }


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */