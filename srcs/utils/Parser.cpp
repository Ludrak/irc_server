#include "Parser.hpp"

/*
** --------------------------------- STATIC ----------------------------------
*/


std::string 				Parser::getParam(std::string command, size_t idx)
{
	size_t nbParam = Parser::nbParam(command);
	if (nbParam == 0 || idx >= nbParam)
		return "";
	size_t specialEnd = command.find(" :");
	if (specialEnd != std::string::npos && idx == nbParam - 1)
		return command.substr(specialEnd + 2);
	size_t start = 0;
	size_t end = command.find(' ');
	while (idx > 0)
	{
		--idx;
		start = end + 1;
		end = command.find(' ', start);
	}
	if (end == std::string::npos)
		return command.substr(start);
	else
		return (command.substr(start, end - start));
}


std::list<std::string>	Parser::paramToList(std::string param)
{
	if (param.empty())
		return std::list<std::string>();
	size_t start = 0;
	size_t end = 0;
	std::list<std::string> param_list;
	while (end != std::string::npos)
	{
		end = param.find(',', start);
		if (end == std::string::npos)
			param_list.push_back(param.substr(start));
		else
			param_list.push_back(param.substr(start, end - start));
		start = end + 1;
	}
	return param_list;
}


size_t		 			Parser::nbParam(std::string command)
{
	if (command.empty())
		return 0;
	size_t nbParam = 1;
	size_t specialParam = command.find(" :");
	size_t param = command.find(' ');
	while (param != std::string::npos && param + 1 < command.size())
	{
		nbParam++;
		if (!(param < specialParam ))
			break ;
		param = command.find(' ', param + 1);
	}
	return nbParam;
}

std::string				Parser::extractFirst(std::string & message)
{
	std::string extracted = Parser::getParam(message, 0);
	size_t pos = message.find(' ');
	message = message.substr(pos);
	pos = message.find_first_not_of(' ');
	message = message.substr(pos);
	return extracted;
}

bool					Parser::validUser(std::string username)
{
	if (username.empty() || username.size() > 100)
		return false;
	if (!(isalpha(username.at(0)) || _isSpecial(username.at(0))))
		return false;
	for (std::string::size_type i = 1; i < username.size(); ++i)
	{
		if ((username[i] == '\0') || (username[i] == '\r') || (username[i] == '\n') || (username[i] == ' ') || (username[i] == '@'))
			return false;
	}
	return true;
}


bool					Parser::validNickname(std::string nick)
{
	if (nick.empty() || nick.size() > 9)
		return false;
	if (!(isalpha(nick.at(0)) || _isSpecial(nick.at(0))))
		return false;
	for (std::string::size_type i = 1; i < nick.size(); ++i)
	{
		if (!(isalnum(nick[i]) || _isSpecial(nick[i]) || nick[i] == '-'))
			return false;
	}
	return true;
}

bool					Parser::validChannelName(std::string channelName)
{
	//REVIEW see for more correct parsing
	if (channelName.size() < 2 || channelName.size() > 50)
		return false;
	if (!((channelName.at(0) == '&') 
		|| (channelName.at(0) == '#') 
		|| (channelName.at(0) == '+') 
		|| (channelName.at(0) == '!')))
		return false;
	for (std::string::size_type i = 1; i < channelName.size(); ++i)
	{
		if (channelName[i] == ','
		||	channelName[i] == ' '
		||	channelName[i] == 0x07)
			return false;
	}
	return true;
}

bool					Parser::validVersionName(std::string version)
{
	if (version.size() < 4 || version.size() > 14)
		return false;
	for (uint i = 0; i < 4 ; ++i)
	{
		if (isdigit(version[i]) == false)
			return false;
	}
	return true;
}

bool					Parser::validPASSflags(std::string flags)
{
	if (flags.size() > 100)
		return false;
	size_t pos = flags.find('|');
	if (pos == std::string::npos)
		return false;
	if (pos != 0 && flags.compare(0, pos, "IRC") != 0)
		return false;
	return true;
}

bool					Parser::_isSpecial(char c)
{
	if ((c >= 91 && c <= 96) || (c >= 123 && c <= 125))
		return true;
	return false;
}

bool 					Parser::isRegularFile(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

/* ************************************************************************** */