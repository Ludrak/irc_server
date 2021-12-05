#ifndef COMMANDLIST_HPP
# define COMMANDLIST_HPP

# include "ACommand.hpp"

# include "Client.hpp"
# include "Server.hpp"

class CommandList : public ACommand
{

	public:

		CommandList(CommandHandler & handler);
		~CommandList();

		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

};

#endif /* ***************************************************** COMMANDLIST_HPP */