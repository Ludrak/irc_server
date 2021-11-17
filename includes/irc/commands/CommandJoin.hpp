#ifndef COMMANDJOIN_HPP
# define COMMANDJOIN_HPP

# include <iostream>
# include <string>
# include "ACommand.hpp"

class CommandJoin : public ACommand
{

	public:

		CommandJoin(CommandHandler & handler);
		~CommandJoin();

		CommandJoin &		operator=( CommandJoin const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:

};

#endif /* ***************************************************** COMMANDJOIN_HPP */