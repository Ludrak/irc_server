#ifndef COMMANDPASS_HPP
# define COMMANDPASS_HPP

# include <iostream>
# include <string>
# include "ACommand.hpp"

class CommandPass : public ACommand
{

	public:

		CommandPass();
		~CommandPass();

		CommandPass &		operator=( CommandPass const & rhs );
		uint				operator()(AEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:

};

std::ostream &			operator<<( std::ostream & o, CommandPass const & i );

#endif /* ***************************************************** COMMANDPASS_HPP */