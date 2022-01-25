#include <string>
#include "ASockHandler.hpp"

ASockHandler::ASockHandler()
{
	Logger::core("ASockHandler constructor");
}

/** @param sock : allocated SockStream* */
void            ASockHandler::addSocket(SockStream &sock)
{
    this->_sockets.insert(std::make_pair(sock.getSocket(), &sock));
}

void            ASockHandler::delSocket(const SockStream &sock)
{
	this->_sockets.erase(sock.getSocket());
	delete &sock;
}

SockStream      *ASockHandler::getSocket(const ushort socket)
{
    return (this->_sockets[socket]);
}
 

void		    ASockHandler::sendPackage(Package *pkg, SockStream &recipient)
{
	Logger::core("added package " + ntos(pkg) + " to pending list of " + recipient.getIP());
	Logger::core("Package data: [" + pkg->getData() + "] (" + ntos(pkg->getData().size()) + ")");
#ifdef	POLL
	recipient.setPollEvent(POLLOUT);
#elif	defined(EPOLL)
#elif	defined(SELECT)
	recipient.selectIO(SELECT_IO_WRITE);
#elif	defined(KQUEUE)
	for (std::vector<struct kevent>::iterator it = this->_k_events.begin(); it != this->_k_events.end(); ++it)
		if (it->ident == recipient.getSocket())
		{
			recipient.delkQueueEvents(this->_kq, this->_k_events, *it, EVFILT_READ);
			recipient.setkQueueEvents(this->_kq, this->_k_events, *it, EVFILT_WRITE);
		}
#endif
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
