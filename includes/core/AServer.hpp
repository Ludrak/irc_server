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
	class AddressBindException : public std::exception
	{
		virtual const char	*what() const throw()
		{
			//TODO implement errno error
			return "socket binding failed";
		}
	};
	class ListenException : public std::exception
	{
		virtual const char	*what() const throw()
		{
			//TODO implement errno error
			return "server cannot listen";
		}
	};
	public:

		AServer();
		AServer( AServer const & src );
		virtual ~AServer();

		AServer &		operator=( AServer const & rhs );

		bool					run( void );
		Channel					*getChannel(int		ChannelId);
		void					load_config_file(std::string path_config_file);

		//add param for initialising server
		uint					getMaxConnection( void ) const;
		void					setMaxConnection( uint nb);

	private:
		uint					_max_connection;
		std::list<Client>		_clients;
		bool					_init_server( void );
		virtual Client			*_acceptConnection(ASockStream &socket_client);

};

std::ostream &			operator<<( std::ostream & o, AServer const & i );

#endif /* ********************************************************* ASERVER_H */