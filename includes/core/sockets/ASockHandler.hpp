
#ifndef ASOCKHANDLER_HPP
# define ASOCKHANDLER_HPP

#include <iostream>
#include <map>
#include <poll.h>
#include "SockStream.hpp"

class ASockHandler
{
    public:
        ASockHandler(void);

        void                addSocket(SockStream *const &sock);
        virtual void        delSocket(SockStream *sock);
        SockStream          *getSocket(const ushort socket);

        void				sendPackage( Package *package, SockStream &recipient);
		void				broadcastPackage( const Package &package, const SockStream *except = NULL);

    protected:
        std::map<ushort, SockStream *> _sockets;
};


# endif // SOCKHANDLER_HPP