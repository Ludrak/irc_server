#ifndef ASERVER_HPP
# define ASERVER_HPP

class AServer;

# include <iostream>
# include <string>
# include <vector>
# include <poll.h>
# include <map>
# include <netdb.h>
# include "ASockManager.hpp"

class AServer : public virtual ASockManager
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
					return ("failed to read incoming data: ");
				}
		};
		
		AServer( const std::string &host="127.0.0.1", const std::string &ssl_cert_path="", const std::string &ssl_key_path="" );
		virtual ~AServer();

		void						loadConfigFile(std::string pathConfigFile);

		bool						listenOn( ushort port, IProtocol &protocol, const bool useTLS=false );
		void						run();

		void						disconnect( SockStream &client );

		uint						getMaxConnection( void ) const;
		void						setMaxConnection( uint nb);

	protected:
		std::string					_host;
		bool						_running;

		static uint					_defaultMaxConnections;

		virtual t_pollevent			_onPollEvent(int socket, int event);
		virtual void				_onClientJoin(SockStream &s) = 0;
		virtual void				_onClientRecv(SockStream &s, Package &pkg) = 0;
		virtual void				_onClientQuit(SockStream &s) = 0;

	private:
		uint						_maxConnections;

		// AServer( const std::string &host, const std::string &ssl_pem_path="" );

		AServer&					operator=( AServer const & rhs );

		t_pollevent					_pollFromServer(int socket, int event);
		t_pollevent					_pollFromClients(int socket, int event);
		t_pollevent					_pollInClients(SockStream & sock);
		t_pollevent					_pollOutClients(SockStream & sock);
};

#endif /* ********************************************************* ASERVER_H */