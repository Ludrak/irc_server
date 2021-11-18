
#include "ASockManager.hpp"

ASockManager::ASockManager() : ASockHandler()
{
	Logger::debug("ASockManager constructor");
}

void            ASockManager::delSocket(const SockStream &sock)
{
#ifndef KQUEUE
	for (std::vector<pollfd>::iterator it = this->_poll_fds.begin(); it != this->_poll_fds.end(); it++)
	{
		if (it->fd == sock.getSocket())
		{
			this->_poll_fds.erase(it);
			break ;
		}
	}
#endif
	ASockHandler::delSocket(sock);
}

#ifndef KQUEUE
void            ASockManager::run( void )
{
    struct pollfd pfd;
	Logger::info("Manager running.");
    while (true)
    {
        //std::vector<struct pollfd>  poll_fds;
        this->_poll_fds = std::vector<struct pollfd>();

        /* Setting all events for next poll */
        for (std::map<ushort, SockStream*>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); ++it)
        {
			pfd.fd = it->first;
			pfd.events = it->second->getPollEvents();
			pfd.revents = 0;
			this->_poll_fds.push_back(pfd);
        }

        /* waiting events from fds */
        if (poll(this->_poll_fds.data(), this->_poll_fds.size(), 0) < 0)
        {
            Logger::error(std::string("poll error: ") + strerror(errno));
            return ;
        }
        size_t sz = 0;
        /* execute events handler */
        for (std::vector<struct pollfd>::iterator it = this->_poll_fds.begin(); it != this->_poll_fds.end(); ++it)
        {
            if (it->revents != 0)
            {
				sz = this->_sockets.size();
				try {
                	this->_onPollEvent(it->fd, it->revents);
				}
				catch (Package::SizeExceededException & e)
				{
					Logger::warning(e.what());
				}
                if (this->_sockets.size() != sz)
                    break;
            }
        }
    }
}
#else
void            ASockManager::run( void )
{
    struct kevent                new_event;
    int                          kq;

    /* init kqueue */
    kq = kqueue();
    if (kq == -1)
    {
        Logger::error("kqueue() failed");
        return ;
    }

    /* add events to list */
    for (std::map<ushort, SockStream*>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); ++it)    
    {   
        EV_SET(&new_event, it->first, EVFILT_READ, EV_ADD, 0, 0, NULL);
        this->_k_events.push_back(new_event);
    }

	Logger::info("Manager running using kqueue");
    for (;;)
    {
        struct kevent events_triggered[this->_k_events.size()];
        int nev = kevent(kq, this->_k_events.data(), this->_k_events.size(), events_triggered, this->_k_events.size(), NULL);

        if (nev == -1)
            Logger::error("kevent() failed: " + ntos(strerror(errno)));

        size_t sz = this->_sockets.size();
        for (int i = 0; i < nev; i++)
        {
            if (events_triggered[i].flags & EV_ERROR)
            {
                Logger::critical("event error: " + ntos(strerror(events_triggered[i].data)));
                return ;
            }
            try {
                this->_onPollEvent(events_triggered[i].ident, events_triggered[i].filter);
            }
            catch (Package::SizeExceededException & e)
            {
                Logger::warning(e.what());
            }
            if (this->_sockets.size() != sz)
                break;
        }
    }
}
#endif
