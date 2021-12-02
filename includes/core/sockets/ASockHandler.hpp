
#ifndef ASOCKHANDLER_HPP
# define ASOCKHANDLER_HPP

# ifdef __linux__
#  include <sys/time.h>
#  include <event.h>
# else
#  include <sys/event.h>
# endif

# include <iostream>
# include <map>
# include <poll.h>
# include "Logger.hpp"
# include "ntos.hpp"
# include "SockStream.hpp"

class ASockHandler
{
    public:
        ASockHandler(void);

		void            	addSocket(SockStream &sock);

        virtual void        delSocket(const SockStream &sock);
        SockStream          *getSocket(const ushort socket);

        void				sendPackage( Package *package, SockStream &recipient);
		void				broadcastPackage( const Package &package, const SockStream *except = NULL);

    protected:
        std::map<ushort, SockStream *>  _sockets;
#ifdef KQUEUE
        int                             _kq;
        std::vector<struct kevent>      _k_events;
#endif
};

# endif // SOCKHANDLER_HPP
