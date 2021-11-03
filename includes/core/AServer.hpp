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
					return ("socket binding failed: ");
				}
		};
		
		class ListenException : public std::exception
		{
			public:
				virtual const char	*what() const throw()
				{
					return ("server cannot listen: ");
				}
		};

		class PollException : public std::exception
		{
			public:
				virtual const char	*what() const throw()
				{
					return ("failed to poll on fds: ");
				}
		};

		class IncomingConnectionException : public std::exception
		{
			public:
				virtual const char	*what() const throw()
				{
					return ("Incoming connection failed: ");
				}
		};

		class PacketReadingException : public std::exception
		{
			public:
				virtual const char	*what() const throw()
				{
					return ("fail to read incoming data: ");
				}
		};
		
		AServer(IProtocol & protocol, const std::string &host = "127.0.0.1", int port = 8080);
		virtual ~AServer();

		bool						run( void );
		void						loadConfigFile(std::string pathConfigFile);

		void						sendPackage( Package *package, SockStream &recipient);
		void						sendAll( const Package &package, const SockStream *except = NULL);

		std::map<int, SockStream*>	&getClients();

		uint						getMaxConnection( void ) const;
		void						setMaxConnection( uint nb);

	protected:
		static uint					_defaultMaxConnections;
		std::map<int, SockStream *>	_clients;

		virtual SockStream&			_onClientJoin(SockStream &s) = 0;
		virtual void				_onClientRecv(SockStream &s, Package &pkg) = 0;
		virtual void				_onClientQuit(SockStream &s) = 0;

	private:
		AServer( AServer const & src );

		AServer&					operator=( AServer const & rhs );

		bool						_initServer( void );
		SockStream&					_acceptConnection( void );

		uint						_maxConnections;
};

#endif /* ********************************************************* ASERVER_H */