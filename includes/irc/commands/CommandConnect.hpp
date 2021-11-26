#ifndef COMMANDCONNECT_HPP
# define COMMANDCONNECT_HPP

# include "ACommand.hpp"

class CommandConnect : public ACommand
{

	public:

		CommandConnect(CommandHandler & handler);
		~CommandConnect();

		CommandConnect &	operator=( CommandConnect const & rhs );
		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:
		uint				_forwardRequest(NetworkEntity & executor, std::string & target, std::string & port, std::string & forward);


};

#endif /* ***************************************************** COMMANDCONNECT_HPP */