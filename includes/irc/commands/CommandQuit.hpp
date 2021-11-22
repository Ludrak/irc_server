#ifndef COMMANDQUIT_HPP
# define COMMANDQUIT_HPP

# include "ACommand.hpp"

class CommandQuit : public ACommand
{

	public:

		CommandQuit(CommandHandler & handler);
		~CommandQuit();

		CommandQuit &		operator=( CommandQuit const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:

};

#endif /* ***************************************************** COMMANDQUIT_HPP */