#ifndef COMMANDOPER_HPP
# define COMMANDOPER_HPP

# include "ACommand.hpp"

class CommandOper : public ACommand
{

	public:

		CommandOper(CommandHandler & handler);
		~CommandOper();

		CommandOper &		operator=( CommandOper const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:

};

#endif /* ***************************************************** COMMANDOPER_HPP */