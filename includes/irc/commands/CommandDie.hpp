#ifndef COMMANDDIE_HPP
# define COMMANDDIE_HPP

# include "ACommand.hpp"

class CommandDie : public ACommand
{

	public:

		CommandDie(CommandHandler & handler);
		~CommandDie();

		CommandDie &	operator=( CommandDie const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:
		uint				_kickAllConnections( void);

};

#endif /* ***************************************************** COMMANDDIE_HPP */