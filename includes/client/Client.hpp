#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "AEntitie"
# include "ASockstream"
# include "Server.hpp"

class Client : public AEntitie, public ASockstream
{

	public:

		Client();
		Client(Server &master);
		Client( Client const & src );
		~Client();

		Client &		operator=( Client const & rhs );
		typename bool	(*Client::Operations)(Client & client, std::string str) Operation;

		Server		&getServer
	private:
		//socket
		std::string				_nickname;
		std::map<Operations>	_op;
		Server 					&_master;
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */