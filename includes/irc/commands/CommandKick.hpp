#ifndef COMMANDKICK_HPP
# define COMMANDKICK_HPP

# include "ACommand.hpp"

# include "Client.hpp"
# include "Server.hpp"

class CommandKick : public ACommand
{

	public:

		CommandKick(CommandHandler & handler);
		~CommandKick();

		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

};

#endif /* ***************************************************** COMMANDKICK_HPP */