

#ifndef COMMANDMOTD_HPP
# define COMMANDMOTD_HPP

# include <fstream>
# include "ACommand.hpp"

class CommandMotd : public ACommand
{

	public:

		CommandMotd(CommandHandler & handler);
		~CommandMotd();

		CommandMotd &		operator=( CommandMotd const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:

};

#endif /* ***************************************************** COMMANDMOTD_HPP */