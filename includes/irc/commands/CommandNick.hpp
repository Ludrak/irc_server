#ifndef COMMANDNICK_HPP
# define COMMANDNICK_HPP

# include "ACommand.hpp"

# include "Client.hpp"
# include "Server.hpp"

class CommandNick : public ACommand
{

	public:

		CommandNick(CommandHandler & handler);
		~CommandNick();

		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:
		uint				_nickFromUnregistered(UnRegisteredConnection & executor, std::string & nick);
		uint				_nickFromClient(Client & executor, std::string & nick);
		uint				_nickFromServer(Server & executor, std::string & params);

};

#endif /* ***************************************************** COMMANDNICK_H */