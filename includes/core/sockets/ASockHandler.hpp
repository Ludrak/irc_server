
#ifndef ASOCKHANDLER_HPP
# define ASOCKHANDLER_HPP

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
        std::map<ushort, SockStream *> _sockets;
};


# endif // SOCKHANDLER_HPP