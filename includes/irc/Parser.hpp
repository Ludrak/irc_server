#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include <string>

class Parser
{

	public:
		static std::string 		getParam(std::string command, size_t idx);
		static size_t		 	nbParam(std::string command);
		static bool				validNickname(std::string nick);
		static bool				isSpecial(char c);
	private:

};

std::ostream &			operator<<( std::ostream & o, Parser const & i );

#endif /* ********************************************************** PARSER_H */