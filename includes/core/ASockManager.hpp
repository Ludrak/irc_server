#ifndef SOCK_MANAGER_HPP
# define SOCK_MANAGER_HPP

# include <iostream>
# include <map>
# include <poll.h>
# include "SockStream.hpp"
# include "Logger.hpp"

typedef int t_pollevent;
# define POLL_SUCCESS   0
# define POLL_NOTFOUND  1
# define POLL_DELETE    2
# define POLL_NOACCEPT  3

class ASockManager
{
    public:
        ASockManager(void);
        ~ASockManager();

        virtual void        run(void);
        void                addSocket(SockStream *const &sock);
        void                delSocket(SockStream *sock);
        void                getSocket(int socket) const;

    protected:
        std::map<int, SockStream *>   _sockets;

        virtual t_pollevent _onPollEvent(int socket, int event) = 0;
};


# endif // SOCK_MANAGER_HPP