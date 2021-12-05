#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include <string>
# include <list>
# include "Logger.hpp"

# include "AEntity.hpp"
# include "NetworkEntity.hpp"
# include "RelayedEntity.hpp"

# include "Client.hpp"
# include "RelayedClient.hpp"
# include "ClientInfo.hpp"

# include "Server.hpp"
# include "RelayedServer.hpp"
# include "ServerInfo.hpp"

namespace Parser
{
	std::string				getParam(std::string command, size_t idx);
	std::list<std::string>	paramToList(std::string param);
	size_t				 	nbParam(std::string command);
	bool					validNickname(std::string nick);
	bool					validUser(std::string username);
	bool					validChannelName(std::string channelName);
	bool					validVersionName(std::string version);
	bool					validPASSflags(std::string flags);
	bool 					isRegularFile(const char *path);

	bool					_isSpecial(char c);
};

#endif /* ********************************************************** PARSER_H */