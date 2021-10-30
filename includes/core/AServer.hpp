#ifndef ASERVER_HPP
# define ASERVER_HPP

class AServer;
class Channel;

# include <iostream>
# include <string>
# include <vector>
# include "poll.h"
# include "SockStream.hpp"
# include "Client.hpp"
# include "Channel.hpp"

class AServer : public SockStream
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

	class PollException : public std::exception
	{
		virtual const char	*what() const throw()
		{
			//TODO implement errno error
			return "failed to poll on fds";
		}
	};

		class IncomingConnectionException : public std::exception
	{
		virtual const char	*what() const throw()
		{
			//TODO implement errno error
			return "Incoming connection refused/failed.";
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
	protected:
		static uint				default_max_connections;

	private:
		std::list<Client>		_clients;
		bool					_init_server( void );
		Client					_acceptConnection( void );
		uint					_max_connection;

};

std::ostream &			operator<<( std::ostream & o, AServer const & i );

#endif /* ********************************************************* ASERVER_H */