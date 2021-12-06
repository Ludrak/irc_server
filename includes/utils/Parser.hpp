#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include <string>
# include <list>
# include "Logger.hpp"
# include <sys/stat.h>


namespace Parser
{
	std::string				getParam(std::string command, size_t idx);
	std::list<std::string>	paramToList(std::string param);
	size_t				 	nbParam(std::string command);
	std::string				extractFirst(std::string & message);
	bool					validNickname(std::string nick);
	bool					validUser(std::string username);
	bool					validChannelName(std::string channelName);
	bool					validVersionName(std::string version);
	bool					validPASSflags(std::string flags);
	bool 					isRegularFile(const char *path);

	bool					_isSpecial(char c);
};

#endif /* ********************************************************** PARSER_H */