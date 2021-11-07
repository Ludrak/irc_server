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
    
    protected:
        virtual void    _onConnect (const SockStream &server) = 0;
        virtual void    _onRecv(const SockStream &server, const Package &pkg) = 0;
        virtual void    _onQuit(const SockStream &server) = 0;
        virtual void    _onKicked(const SockStream &server) = 0;

    private:
        t_pollevent     _onPollEvent(int socket, int event);

        t_pollevent		_pollFromServers(SockStream *const sock, int event);
        t_pollevent		_pollInClients(SockStream *const sock);
		t_pollevent		_pollOutClients(SockStream *const sock);
};

#endif // ACLIENT_HPP