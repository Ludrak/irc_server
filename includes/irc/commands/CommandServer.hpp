
#ifndef COMMANDSERVER_HPP
# define COMMANDSERVER_HPP

# include <iostream>
# include <string>
# include <iomanip>
# include "ACommand.hpp"

class CommandServer : public ACommand
{

	public:

		CommandServer(CommandHandler & handler);
		~CommandServer();

		CommandServer &		operator=( CommandServer const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:
		uint				_sendDataToServer(Server & new_serv);

};

#endif /* ***************************************************** COMMANDSERVER_HPP */