#ifndef COMMANDTOPIC_HPP
# define COMMANDTOPIC_HPP

# include "ACommand.hpp"

class CommandTopic : public ACommand
{

	public:

		CommandTopic(CommandHandler & handler);
		~CommandTopic();

		CommandTopic &		operator=( CommandTopic const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:

};

#endif /* ***************************************************** COMMANDTOPIC_HPP */