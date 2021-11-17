#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP

class IRCServer;
class ACommand;

# include <iostream>
# include <string>
# include <map>
# include "NetworkEntity.hpp"
# include "Logger.hpp"

class CommandHandler
{
	public:

		CommandHandler(IRCServer & server);
		CommandHandler( CommandHandler const & src );
		~CommandHandler();

		CommandHandler &		operator=( CommandHandler const & rhs );
		
		IRCServer&		getServer( void );

		template<typename T>
		void			addCommand( const std::string name)	
		{
			if (this->_commands.count(name) == 0)
				this->_commands.insert(std::make_pair(name, new T(*this)));
			else
				Logger::critical("Handler: Command " + name + " is inserted twice.");
		}		
		uint			handle(NetworkEntity & executor, std::string data);

	private:
		
		IRCServer&								_server;
		/* Operations lists */
		std::map<std::string, ACommand*>		_commands;
};

# include "IRCServer.hpp"
# include "ACommand.hpp"

//TODO implement it for log in full debug
std::ostream &			operator<<( std::ostream & o, CommandHandler const & i );

#endif /* ************************************************** COMMANDHANDLER_HPP */