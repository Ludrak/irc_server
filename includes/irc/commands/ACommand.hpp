#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP


class	IRCServer;
class	CommandHandler;
# include <iostream>
# include <string>
# include "Server.hpp"
# include "status.hpp"

class ACommand
{
	public:

		ACommand(CommandHandler & handler);
		ACommand( ACommand const & src );
		virtual ~ACommand();

		ACommand &			operator=( ACommand const & rhs );
		virtual uint		operator()(NetworkEntity & executor, std::string params) = 0;
		
		IRCServer&			getServer( void );
		CommandHandler&		getHandler( void );

		const Server*		getSender( void ) const;
		void				setSender(Server * sender);

		AEntity&			getEmitter( void ) const;
		void				setEmitter(AEntity & client);

		const AEntity*		getClientHost( void ) const;
		void				setClientHost(AEntity * host);

		virtual	bool		hasPermissions(AEntity & executor);

	private:
		Server				*_sender;
		AEntity				*_client;
		AEntity				*_clientHost;
		CommandHandler		&_handler;

};

# include "IRCServer.hpp"
# include "CommandHandler.hpp"

#endif /* ********************************************************* ACOMMAND_HPP */