#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

# include "AServer.hpp"

class IRCServer : public AServer
{

	public:

		IRCServer();
		IRCServer( IRCServer const & src );
		~IRCServer();

		IRCServer &		operator=( IRCServer const & rhs );

	private:
		std::list<Channel>	 	_channels;
		int						_Server_network_socket;

};

std::ostream &			operator<<( std::ostream & o, IRCServer const & i );

#endif /* ******************************************************* IRCSERVER_H */