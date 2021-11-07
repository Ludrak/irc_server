#ifndef ACLIENT_HPP
# define ACLIENT_HPP

class AClient;

# include <iostream>
# include <string>
# include <vector>
# include <poll.h>
# include <map>
# include "SockStream.hpp"
# include "ASockManager.hpp"

class AClient : public virtual ASockManager
{
    public:
        AClient(void);
        virtual ~AClient();

        bool            connectOn(const std::string host, const ushort port, IProtocol &protocol);
        void			sendServerPackage( Package *package, SockStream &server_recipient);
    
    protected:
        virtual void    _onConnect ( SockStream &server) = 0;
        virtual void    _onRecv( SockStream &server,  Package &pkg) = 0;
        virtual void    _onQuit( SockStream &server) = 0;
        virtual void    _onKicked( SockStream &server) = 0;

    private:
        t_pollevent     _onPollEvent(int socket, int event);

        t_pollevent		_pollFromServers(SockStream *const sock, int event);
        t_pollevent		_pollInClients(SockStream *const sock);
		t_pollevent		_pollOutClients(SockStream *const sock);
};

#endif // ACLIENT_HPP