
#include "ASockManager.hpp"

ASockManager::ASockManager() : ASockHandler()
{
	Logger::debug("ASockManager constructor");
}

void            ASockManager::delSocket(const SockStream &sock)
{
#ifdef POLL
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


/**********************************************************************/
/*** SERVER/CLIENT RUN                                              ***/
/*** Defined for multiple architechtures, implemented modes are:    ***/
/*** - POLL                                                         ***/
/*** - SELECT                                                       ***/
/*** - KQUEUE                                                       ***/
/**********************************************************************/



#ifdef POLL
/* POLL                                                                     */
/* Poll is the most cross-platform solution, but eats up all CPU ressources */
void            ASockManager::run( void )
{
    struct pollfd pfd;
	Logger::info("Manager running using poll()");
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



#elif   defined(EPOLL)
/* EPOLL                                                                    */
/* Epoll is the linux's poll, but way faster than regular poll              */
/* (however not currently implemented)                                      */
void            ASockManager::run( void )
{
    /** STILL UNSUPPORTED **/
    Logger::critical ("Cannot run with EPOLL for now: not implemented");
}



#elif   defined(SELECT)
/* SELECT                                                                   */
/* Select is probably the best option for cross-plateform between linux,    */
/* macos and bsd, however if only bsd or linux are used, kqueue and epoll   */
/* respectivy should be the best options performance-wise                   */
void            ASockManager::run( void )
{
    fd_set  read_efds;
    fd_set  write_efds;
    int     big_fd = 0;

    Logger::info("Manager running using select()");
    for (;;)
    {
        /* clearing fd sets */
        FD_ZERO(&read_efds);
        FD_ZERO(&write_efds);

        /* Setting all events for next selection */
        for (std::map<ushort, SockStream*>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); ++it)
        {
            if (it->second->getSocket() > big_fd)
                big_fd = it->second->getSocket();
            if (it->second->getSelectedIOs() & SELECT_IO_READ)
                FD_SET(it->second->getSocket(), &read_efds);
            if (it->second->getSelectedIOs() & SELECT_IO_WRITE)
                FD_SET(it->second->getSocket(), &write_efds);
        }

        /* selecting which fds are available for IO operations */
        int n_changes = select(big_fd + 1, &read_efds, &write_efds, NULL, NULL);
        if (n_changes == -1)
        {
            Logger::critical("select() failed: " + ntos(strerror(errno)));
            return ;
        }

        /* getting io events from sets */
        for (std::map<ushort, SockStream*>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); ++it)
        {
            int events = 0;
            if (FD_ISSET(it->second->getSocket(), &read_efds))
            {
                /* read available */
                events |= SELECT_IO_READ;
            }
            if (FD_ISSET(it->second->getSocket(), &write_efds))
            {
                /* write available */
                events |= SELECT_IO_WRITE;
            }
            if (events > 0)
            {
                size_t sz = this->_sockets.size();
				try {
                	this->_onPollEvent(it->second->getSocket(), events);
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



#elif   defined(KQUEUE)
/* KQUEUE                                                                               */
/* kqueue is the best option for compiling on a BSD OS, however, no specific            */
/* optimisations for BSD are done (EVFLT_VNODE instead of EVFLT_READ & EVFLT_WRITE)     */
/* this option is although usefull on macos / linux / bsd until their specific options  */
/* are implemented (or for cross plateform program built for these OS's)                */
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

	Logger::info("Manager running using kqueue()");
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
