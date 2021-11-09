#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include <string>
# include <list>

class Parser
{
	public:
		static std::string				getParam(std::string command, size_t idx);
		static std::list<std::string>	paramToList(std::string param);
		static size_t				 	nbParam(std::string command);
		static bool						validNickname(std::string nick);
		static bool						validChannelName(std::string channelName);
		static bool						isSpecial(char c);
	private:

};

#endif /* ********************************************************** PARSER_H */