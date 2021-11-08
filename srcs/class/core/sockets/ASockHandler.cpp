
#include "ASockHandler.hpp"

ASockHandler::ASockHandler() { }

/** @param sock : allocated SockStream* */
void            ASockHandler::addSocket(SockStream *const &sock)
{
    this->_sockets.insert(std::make_pair(sock->getSocket(), sock));
}

void            ASockHandler::delSocket(SockStream *sock)
{
    SockStream *s = this->_sockets[sock->getSocket()];
    if (s)
    {
        this->_sockets.erase(sock->getSocket());
        delete s;
    }
}

SockStream      *ASockHandler::getSocket(const ushort socket)
{
    return (this->_sockets[socket]);
}


void		    ASockHandler::sendPackage( Package *pkg, SockStream &recipient)
{
	recipient.setPollEvent(POLLOUT);
	pkg->setRecipient(&recipient);
	recipient.getPendingData().push_back(pkg);
}



void			ASockHandler::broadcastPackage( const Package &package, const SockStream *except )
{
	for (std::map<ushort, SockStream *>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++)
	{
		if (!except || it->second != except)
			this->sendPackage(new Package(package), *it->second);
	}
}
