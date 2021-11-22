#ifndef COMMANDSQUIT_HPP
# define COMMANDSQUIT_HPP

# include "ACommand.hpp"

class CommandSquit : public ACommand
{

	public:

		CommandSquit(CommandHandler & handler);
		~CommandSquit();

		CommandSquit &		operator=( CommandSquit const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

		uint				_quitServer(const AEntity & emitter, std::string & serverName, std::string & comment);

	private:

};

#endif /* ***************************************************** COMMANDSQUIT_HPP */