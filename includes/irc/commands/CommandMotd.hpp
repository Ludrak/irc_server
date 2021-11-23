

#ifndef COMMANDMOTD_HPP
# define COMMANDMOTD_HPP

# include <fstream>
# include <sys/stat.h>
# include "ACommand.hpp"

class CommandMotd : public ACommand
{

	public:

		CommandMotd(CommandHandler & handler);
		~CommandMotd();

		CommandMotd &		operator=( CommandMotd const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);
		uint				operator()(NetworkEntity & executor, std::string params, bool useShort);

		bool				hasPermissions(AEntity & executor);
		
		int					isRegularFile(const char *path);

	private:
		bool				_shortEnabled;
};

#endif /* ***************************************************** COMMANDMOTD_HPP */