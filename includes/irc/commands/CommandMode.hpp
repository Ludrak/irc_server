#ifndef COMMANDMODE_HPP
# define COMMANDMODE_HPP

# include "ACommand.hpp"

# include "Client.hpp"
# include "Server.hpp"

class CommandMode : public ACommand
{

	public:

		CommandMode(CommandHandler & handler);
		~CommandMode();

		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:
		/* OLD */
		uint				_modeFromClient(Client & executor, std::string & mode);
		uint				_modeFromServer(Server & executor, std::string & params);
		void				modeForUser(NetworkEntity & executor, std::string uid, std::string mode);
		void				modeForChannel(uint nbParam, NetworkEntity & executor, std::string params);

		/* NEW */
		void				userMode(uint nb_params, std::string params, NetworkEntity& executor);
		void				channelMode(uint nb_params, std::string params, NetworkEntity& executor);
};

#endif /* ***************************************************** COMMANDMODE_HPP */