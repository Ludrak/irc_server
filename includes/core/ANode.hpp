#ifndef ANODE_HPP
#define ANODE_HPP

# include "AServer.hpp"
# include "AClient.hpp"

class ANode : public AServer, public AClient
{
	public:
		ANode(void);
		~ANode(void);


	protected:
	//ANode
		t_pollevent					_onPollEvent(int socket, int event);

	//AServer protected

		//Called when a new client try to etablish a connection on one server socket
		virtual void				_onClientJoin(SockStream &s) = 0;

		//Called when receive a package from a connected client
		virtual void				_onClientRecv(SockStream &s, Package &pkg) = 0;

		//Called when a client is disconnected
		virtual void				_onClientQuit(SockStream &s) = 0;

	//AClient protected

		//Called when receive data from a Server where we are connected
		virtual void				_onRecv( SockStream &server,  Package &pkg) = 0;
		
		//Called when a new connection to a remote server is started
		virtual void				_onConnect ( SockStream &server) = 0;
		
		//Called when we want to stop a connection to a remote server
		virtual void				_onQuit( SockStream &server) = 0;
		
		//Called when we are kicked from a remote server
		virtual void				_onKicked( SockStream &server) = 0;

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

	//Hidden accessible members
		// std::map<int, SockStream *>	_sockets;
		// std::string					_host;
		// bool							_running;
		// static uint					_defaultMaxConnections;
	
	//Et pour finir grosse blague: Il me semble que j'ai pas besoin de redefinir ici toute les virtuel pures, 
	//donc en fait dans ANode il  va y avoir juste Constructeur, destructeur et _onPollEvent et c'est tout x)
};

#endif 
