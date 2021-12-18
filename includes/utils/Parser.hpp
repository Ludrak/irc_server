#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include <string>
# include <list>
# include "Logger.hpp"
# include <sys/stat.h>


namespace Parser
{
	std::string				getParam(const std::string &command, size_t idx, const std::string sep = " ");
	size_t				 	nbParam(const std::string & command, const std::string sep = " ");
	std::list<std::string>	paramToList(const std::string & param, const std::string sep = ",");
	std::string				extractFirst(std::string & message, const std::string sep = " ");
	bool					validNickname(const std::string & nick);
	bool					validUser(const std::string & username);
	bool					validChannelName(const std::string &channelName);
	bool					validHostname(const std::string & hostname);
	bool					validShortname(const std::string & hostname);
	bool					validVersionName(const std::string & version);
	bool					validPASSflags(const std::string & flags);
	bool 					isRegularFile(const char *path);
	bool					isDirectory(const char *path);

	bool					_isSpecial(char c);
};

#endif /* ********************************************************** PARSER_H */