#ifndef COMMANDPONG_HPP
# define COMMANDPONG_HPP

# include "ACommand.hpp"

class CommandPong : public ACommand
{

	public:

		CommandPong(CommandHandler & handler);
		~CommandPong();

		CommandPong &	operator=( CommandPong const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:

};

#endif /* ***************************************************** COMMANDPONG_HPP */