#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP

class IRCServer;

# include <iostream>
# include <string>
# include "ACommand.hpp"
# include "Logger.hpp"

class CommandHandler
{
	public:

		CommandHandler(IRCServer & server);
		CommandHandler( CommandHandler const & src );
		~CommandHandler();

		CommandHandler &		operator=( CommandHandler const & rhs );
		
		template<typename T>
		void			addCommand( const std::string name);
		
		uint			handle(AEntity & executor, std::string data);
	private:
		// typedef uint	(IRCServer::*Operations)(AEntity & exector, std::string params);
		
		IRCServer&							_server;
		/* Operations lists */
		std::map<std::string, ACommand*>		_commands;
};

# include "IRCServer.hpp"

//TODO implement it for log in full debug
std::ostream &			operator<<( std::ostream & o, CommandHandler const & i );

#endif /* ************************************************** COMMANDHANDLER_HPP */