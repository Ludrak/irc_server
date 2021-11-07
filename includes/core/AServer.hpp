#ifndef ASERVER_HPP
# define ASERVER_HPP

class AServer;

# include <iostream>
# include <string>
# include <vector>
# include <poll.h>
# include <map>
# include "SockStream.hpp"
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
					return ("fail to read incoming data: ");
				}
		};
		
		AServer( const std::string &host = "127.0.0.1" );
		virtual ~AServer();

		void						loadConfigFile(std::string pathConfigFile);

		bool						listenOn( ushort port, IProtocol &protocol );
		void						run();

		void						sendPackage( Package *package, SockStream &recipient);
		void						sendAll( const Package &package, const SockStream *except = NULL);
		void						kick( SockStream &client );

		uint						getMaxConnection( void ) const;
		void						setMaxConnection( uint nb);

	protected:
		std::string					_host;
		static uint					_defaultMaxConnections;

		virtual void				_onClientJoin(SockStream &s) = 0;
		virtual void				_onClientRecv(SockStream &s, Package &pkg) = 0;
		virtual void				_onClientQuit(SockStream &s) = 0;

	private:
		uint						_maxConnections;

		AServer( AServer const & src );
		AServer&					operator=( AServer const & rhs );


		t_pollevent					_onPollEvent(int socket, int event);
		/* branchless routing for _onPollEvent() */
 		__attribute__((always_inline))
		inline t_pollevent			_pollFromServer(int socket, int event);
		__attribute__((always_inline))
		inline t_pollevent			_pollFromClients(int socket, int event);
		__attribute__((always_inline))
		inline t_pollevent			_pollInClients(SockStream *const sock);
		__attribute__((always_inline))
		inline t_pollevent			_pollOutClients(SockStream *const sock);
};

#endif /* ********************************************************* ASERVER_H */