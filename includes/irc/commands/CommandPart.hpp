#ifndef COMMANDPART_HPP
# define COMMANDPART_HPP

# include "ACommand.hpp"

class CommandPart : public ACommand
{

	public:

		CommandPart(CommandHandler & handler);
		~CommandPart();

		CommandPart &		operator=( CommandPart const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:

};

#endif /* ***************************************************** COMMANDPART_HPP */