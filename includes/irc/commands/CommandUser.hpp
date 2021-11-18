#ifndef COMMANDUSER_HPP
# define COMMANDUSER_HPP

# include <iostream>
# include <string>
# include "ACommand.hpp"

class CommandUser : public ACommand
{

	public:

		CommandUser(CommandHandler & handler);
		~CommandUser();

		CommandUser &		operator=( CommandUser const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);
		uint				_commandUSERunknown(UnRegisteredConnection & executor, std::string params);

	private:

};

#endif /* ***************************************************** COMMANDUSER_HPP */