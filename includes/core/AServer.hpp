#ifndef ASERVER_HPP
# define ASERVER_HPP

class AServer;
class Channel;

# include <iostream>
# include <string>
# include "ASockStream.hpp"
# include "Client.hpp"
# include "Channel.hpp"

class AServer : public ASockStream
{

	public:

		AServer();
		AServer( AServer const & src );
		virtual ~AServer();

		AServer &		operator=( AServer const & rhs );

		Channel				*getChannel(int		ChannelId);
		void				load_config_file(std::string path_config_file);
		//add param for initialising server


	private:
		std::list<Client>	 _clients;
		virtual bool		_init_server( void ) = 0;
		virtual Client		*_acceptConnection(ASockStream &socket_client);
};

std::ostream &			operator<<( std::ostream & o, AServer const & i );

#endif /* ********************************************************* ASERVER_H */