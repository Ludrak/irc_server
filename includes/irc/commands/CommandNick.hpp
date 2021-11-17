#ifndef COMMANDNICK_HPP
# define COMMANDNICK_HPP

# include <iostream>
# include <string>
# include "ACommand.hpp"

class CommandNick : public ACommand
{

	public:

		CommandNick();
		CommandNick( CommandNick const & src );
		~CommandNick();

		CommandNick &		operator=( CommandNick const & rhs );
		uint				operator()(AEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:

};

#endif /* ***************************************************** COMMANDNICK_H */