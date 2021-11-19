#ifndef COMMANDERROR_CPP
# define COMMANDERROR_CPP

# include <iostream>
# include <string>
# include "ACommand.hpp"

class CommandError : public ACommand
{

	public:

		CommandError(CommandHandler & handler);
		~CommandError();

		CommandError &		operator=( CommandError const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:

};

#endif /* ***************************************************** COMMANDERROR_CPP */