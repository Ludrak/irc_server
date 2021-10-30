#ifndef ASERVER_HPP
# define ASERVER_HPP

class AServer;
class Channel;

# include <iostream>
# include <string>
# include <vector>
# include <poll.h>
# include <map>
# include "SockStream.hpp"
# include "Client.hpp"
# include "Channel.hpp"

class AServer : public SockStream
{
	public:
	class AddressBindException : public std::exception
	{
		public:
			virtual const char	*what() const throw()
			{
				return (std::string("socket binding failed: ").append(strerror(errno))).c_str();
			}
	};
	class ListenException : public std::exception
	{
		public:
			virtual const char	*what() const throw()
			{
				return (std::string("server cannot listen: ").append(strerror(errno))).c_str();
			}
	};

	class PollException : public std::exception
	{
		public:
			virtual const char	*what() const throw()
			{
				return (std::string("failed to poll on fds: ").append(strerror(errno))).c_str();
			}
	};

	class IncomingConnectionException : public std::exception
	{
		public:
			virtual const char	*what() const throw()
			{
				return (std::string("Incoming connection failed: ").append(strerror(errno))).c_str();
			}
	};

	class PacketReadingException : public std::exception
	{
		public:
			virtual const char	*what() const throw()
			{
				return (std::string("fail to read incoming data: ").append(strerror(errno))).c_str();
			}
	};

		AServer(const std::string &host = "127.0.0.1", int port = 8080);
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
		AServer( AServer const & src );

		std::map<int, SockStream *>	_clients;
		bool						_init_server( void );
		SockStream&					_acceptConnection( void );
		uint						_max_connection;

};

std::ostream &			operator<<( std::ostream & o, AServer const & i );

#endif /* ********************************************************* ASERVER_H */