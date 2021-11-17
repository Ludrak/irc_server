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

class Parser
{
	public:
		static std::string				getParam(std::string command, size_t idx);
		static std::list<std::string>	paramToList(std::string param);
		static std::string				formatReply(std::string message, std::string target = "", std::string target2 = "");
		static size_t				 	nbParam(std::string command);
		static bool						validNickname(std::string nick);
		static bool						validUser(std::string username);
		static bool						validChannelName(std::string channelName);
		static bool						validVersionName(std::string version);
		static bool						validPASSflags(std::string flags);

		static bool						_isSpecial(char c);
	private:

};

#endif /* ********************************************************** PARSER_H */