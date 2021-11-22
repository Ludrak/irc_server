#ifndef COMMANDPASS_HPP
# define COMMANDPASS_HPP

# include "ACommand.hpp"

class CommandPass : public ACommand
{

	public:

		CommandPass(CommandHandler & handler);
		~CommandPass();

		CommandPass &		operator=( CommandPass const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:

};

#endif /* ***************************************************** COMMANDPASS_HPP */