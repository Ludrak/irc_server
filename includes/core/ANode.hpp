#ifndef ANODE_HPP
#define ANODE_HPP

# include "AServer.hpp"
# include "AClient.hpp"
# include "Logger.hpp"

class ANode : public AServer, public AClient
{
	public:
		ANode(const std::string & host="127.0.0.1", const std::string &ssl_cert_path="", const std::string &ssl_key_path="");
		virtual ~ANode(void);


	protected:
	//ANode
		t_pollevent					_onPollEvent(int socket, int event);

	//Events
		//Called when a new client try to etablish a connection on one server socket
		// virtual void				_onClientJoin(SockStream &s) = 0;

		//Called when receive a package from a connected client
		// virtual void				_onClientRecv(SockStream &s, Package &pkg) = 0;

		//Called when a client is disconnected
		// virtual void				_onClientQuit(SockStream &s) = 0;

		//Called when receive data from a Server where we are connected
		// virtual void				_onRecv( SockStream &server,  Package &pkg) = 0;
		
		//Called when a new connection to a remote server is started
		// virtual void				_onConnect ( SockStream &server) = 0;
		
		//Called when we want to stop a connection to a remote server
		// virtual void				_onQuit( SockStream &server) = 0;
		
		//Called when we are kicked from a remote server
		// virtual void				_onKicked( SockStream &server) = 0;

	//Hidden public methods
		// void        					addSocket(SockStream *const &sock);
		// void        					delSocket(SockStream *sock);
		// void        					getSocket(int socket) const;
		// void							loadConfigFile(std::string pathConfigFile);
		// void							run();
		// void							kick( SockStream &client );
		// void							sendPackage( Package *package, SockStream &recipient);
		// void							sendAll( const Package &package, const SockStream *except = NULL);
		// void							sendServerPackage( Package *package, SockStream &server_recipient);
		// void							setMaxConnection( uint nb);
		// uint							getMaxConnection( void ) const;
		// bool							listenOn( ushort port, IProtocol &protocol );
		// bool							connectOn(const std::string host, const ushort port, IProtocol &protocol);

	//Hidden protected members
		// std::map<int, SockStream *>	_sockets;
		// std::string					_host;
		// bool							_running;
		// static uint					_defaultMaxConnections;
	
};

#endif 
