#ifndef COMMANDVERSION_HPP
# define COMMANDVERSION_HPP

# include "ACommand.hpp"

class CommandVersion : public ACommand
{

	public:

		CommandVersion(CommandHandler & handler);
		~CommandVersion();

		CommandVersion &		operator=( CommandVersion const & rhs );
		uint					operator()(NetworkEntity & executor, std::string params);

		bool					hasPermissions(AEntity & executor);
	
	private:
		uint					_sendVersion(IRCServer &master, AEntity & emitter);


};

#endif /* ***************************************************** COMMANDVERSION_HPP */