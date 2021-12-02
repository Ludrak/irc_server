#ifndef COMMANDNOTICE_HPP
# define COMMANDNOTICE_HPP

# include "ACommand.hpp"

class CommandNotice : public ACommand
{

	public:

		CommandNotice(CommandHandler & handler);
		~CommandNotice();

		CommandNotice &		operator=( CommandNotice const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:

};

#endif /* ***************************************************** COMMANDNOTICE_HPP */