#ifndef ASERVER_HPP
# define ASERVER_HPP

# include <iostream>
# include <string>
# include "ASockstream.hpp"
# include "Client.hpp"
# include "Channel.hpp"

class AServer : public ASockstream
{

	public:

		AServer();
		AServer( AServer const & src );
		~AServer();

		AServer &		operator=( AServer const & rhs );

		Channel				*getChannel(int		ChannelId);
		void				load_config_file(std::string path_config_file);
		//add param for initialising server
		virtual bool		init_server( void ) = 0;
		virtual Client		&acceptConnection(ASockStream &socket_client) = 0;

	private:
		std::list<Clients>	 	_clients;
};

std::ostream &			operator<<( std::ostream & o, AServer const & i );

#endif /* ********************************************************* ASERVER_H */