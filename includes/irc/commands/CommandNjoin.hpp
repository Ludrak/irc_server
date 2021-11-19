#ifndef COMMAND_NJOIN
# define COMMAND_NJOIN

# include <iostream>
# include <string>
# include "ACommand.hpp"

class CommandNjoin : public ACommand
{

	public:

		CommandNjoin(CommandHandler & handler);
		~CommandNjoin();

		CommandNjoin &		operator=( CommandNjoin const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:

};

#endif /* ***************************************************** COMMAND_NJOIN */