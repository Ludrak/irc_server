#ifndef COMMANDUSER_HPP
# define COMMANDUSER_HPP

# include <iostream>
# include <string>
# include "ACommand.hpp"

class CommandUser : public ACommand
{

	public:

		CommandUser(CommandHandler & handler);
		~CommandUser();

		CommandUser &		operator=( CommandUser const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:

};

std::ostream &			operator<<( std::ostream & o, CommandUser const & i );

#endif /* ***************************************************** COMMANDUSER_HPP */