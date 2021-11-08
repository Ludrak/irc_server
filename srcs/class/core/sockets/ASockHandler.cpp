
#include "ASockHandler.hpp"

ASockHandler::ASockHandler() { }

/** @param sock : allocated SockStream* */
void                ASockHandler::addSocket(SockStream *const &sock)
{
    this->_sockets.insert(std::make_pair(sock->getSocket(), sock));
}

void                ASockHandler::delSocket(SockStream *sock)
{
    SockStream *s = this->_sockets[sock->getSocket()];
    if (s)
    {
        this->_sockets.erase(sock->getSocket());
        delete s;
    }
}

SockStream    *ASockHandler::getSocket(const ushort socket)
{
    return (this->_sockets[socket]);
}