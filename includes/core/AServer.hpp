#ifndef ASERVER_HPP
# define ASERVER_HPP

class AServer;

# include <iostream>
# include <string>
# include <vector>
# include <poll.h>
# include <map>
# include "SockStream.hpp"


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
		
		AServer(IProtocol & protocol, const std::string &host = "127.0.0.1", int port = 8080);
		virtual ~AServer();


		bool					run( void );
		void					load_config_file(std::string path_config_file);

		uint					getMaxConnection( void ) const;
		void					setMaxConnection( uint nb);

	protected:
		static uint				_default_max_connections;

		virtual void			_onClientJoin(SockStream &s) = 0;
		virtual void			_onClientRecv(SockStream &s, const Package pkg) = 0;
		virtual void			_onClientQuit(SockStream &s) = 0;

	private:
		AServer( AServer const & src );

		AServer &		operator=( AServer const & rhs );

		bool						_init_server( void );
		SockStream&					_acceptConnection( void );

		std::map<int, SockStream *>	_clients;
		uint						_max_connection;

};

#endif /* ********************************************************* ASERVER_H */