#ifndef COMMANDPRIVMSG_HPP
# define COMMANDPRIVMSG_HPP

# include "ACommand.hpp"

class CommandPrivmsg : public ACommand
{

	public:

		CommandPrivmsg(CommandHandler & handler);
		~CommandPrivmsg();

		CommandPrivmsg &		operator=( CommandPrivmsg const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:

};

#endif /* ***************************************************** COMMANDPRIVMSG_HPP */