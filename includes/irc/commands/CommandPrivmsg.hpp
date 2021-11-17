#ifndef COMMANDPRIVMSG_HPP
# define COMMANDPRIVMSG_HPP

# include <iostream>
# include <string>
# include "ACommand.hpp"

class CommandPrivmsg : public ACommand
{

	public:

		CommandPrivmsg(CommandHandler & handler);
		~CommandPrivmsg();

		CommandPrivmsg &		operator=( CommandPrivmsg const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:

};

std::ostream &			operator<<( std::ostream & o, CommandUser const & i );

#endif /* ***************************************************** COMMANDPRIVMSG_HPP */