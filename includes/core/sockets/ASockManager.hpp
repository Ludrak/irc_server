#ifndef ASOCKMANAGER_HPP
# define ASOCKMANAGER_HPP

#include "ASockHandler.hpp"

typedef int t_pollevent;
# define POLL_SUCCESS   0
# define POLL_NOTFOUND  1
# define POLL_DELETE    2
# define POLL_NOACCEPT  3


class ASockManager : public ASockHandler
{
    public:
        ASockManager(void);

        virtual void        run(void);
        void                delSocket( const SockStream &sock);
    protected:
        std::vector<pollfd>           _poll_fds;

        virtual t_pollevent _onPollEvent(int socket, int event) = 0;
};


# endif // SOCK_MANAGER_HPP