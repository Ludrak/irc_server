#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include <iostream>
# include <string>
# include "Server.hpp"

# include "status.hpp"

class ACommand
{

	public:

		ACommand();
		ACommand( ACommand const & src );
		virtual ~ACommand();

		ACommand &			operator=( ACommand const & rhs );
		virtual uint		operator()(AEntity & executor, std::string params) = 0;
		
		const Server*		getSender( void ) const;
		const AEntity*		getClient( void ) const;
		const AEntity*		getClientHost( void ) const;

		void				setSender(Server * sender);
		void				setClient(AEntity * client);
		void				setClientHost(AEntity * host);

		virtual	bool		hasPermissions(AEntity & executor);

	private:
		Server				*_sender;
		AEntity				*_client;
		AEntity				*_clientHost;

};

#endif /* ********************************************************* ACOMMAND_HPP */