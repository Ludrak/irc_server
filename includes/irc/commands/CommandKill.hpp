#ifndef COMMANDKILL_HPP
# define COMMANDKILL_HPP

# include "ACommand.hpp"

# include "Client.hpp"
# include "Server.hpp"

class CommandKill : public ACommand
{

	public:

		CommandKill(CommandHandler & handler);
		~CommandKill();

		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

};

#endif /* ***************************************************** COMMANDKILL_HPP */