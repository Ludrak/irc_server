# include "IRCServer.hpp"

uint		IRCServer::_commandMODEchannel(AEntity & client, Channel& target, size_t nbParam, std::string params)
{
	//TODO pass this as static function 
	//bascially handled for know
	std::string mode = Parser::getParam(params, 1);
	(void) nbParam;
	if (mode.size() == 1)
	{
		switch (mode[0])
		{
			case 'O':
			{
				//TODO check here if command 'O' come from a server
				// if (client.getType() == AEntity::value_type_server)
				if (target.getCreator() == NULL)
				{
					target.setCreator(client);
					Logger::info("Give 'channel creator' status to " + client.getNickname());
				}
				// else
					// this->_reply(client, RPL_UNIQOPIS);//TODO
				break;
			}
			case 'm':
			{
				Logger::debug("Toogle moderated on channel " + target.getNickname());
				target.toogleMode(M_MODERATED);
				break ;
			}
			default:
			{
				this->_reply(client, ERR_UNKNOWNMODE, mode, target.getNickname());
			}
		}
	}
	else if (mode.size() == 2 && (mode[0] == '+' || mode[0] == '-'))
	{
		//valid
		Logger::warning("valid but not handled yet MODE (channel)");
	}
	else
	{
		Logger::warning("Invalid MODE (" + mode + ") for channel");
		this->_reply(client, ERR_UNKNOWNMODE, mode, target.getNickname());
	}
	return SUCCESS;
}


uint		IRCServer::_commandMODEclient(Client & client, std::string params)
{
	Logger::warning("MODE (client) not supported yet");
	(void) client;
	(void) params;
	return SUCCESS;
}

uint		IRCServer::_commandMODE(AEntity & executor, std::string params)
{
	Logger::debug("<" + ntos(executor.getStream().getSocket()) + "> Command<MODE> with args: " + params );
	size_t nbParam = Parser::nbParam(params);
	if ( nbParam < 2)
		return this->_reply(executor, ERR_NEEDMOREPARAMS, "MODE");
	std::string target_name = Parser::getParam(params, 0);
	if (this->_ircClients.count(target_name) == false)
		return this->_reply(executor, ERR_NOSUCHNICK, target_name);

	AEntity* target = this->_ircClients[target_name];
	if (target->getType() == AEntity::value_type_channel )
		return _commandMODEchannel(executor, *reinterpret_cast<Channel *>(target), nbParam, params);
	else if (target->getType() == AEntity::value_type_client )
		return _commandMODEclient(executor, params);
	Logger::warning("Unhandled type in MODE command");
	return SUCCESS;

}

