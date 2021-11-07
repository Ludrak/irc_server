
#include "ASockManager.hpp"

ASockManager::ASockManager()
{
}

/** @param sock : allocated SockStream* */
void            ASockManager::addSocket(SockStream *const &sock)
{
    this->_sockets.insert(std::make_pair(sock->getSocket(), sock));
}

void            ASockManager::delSocket(SockStream *sock)
{
    this->_sockets.erase(sock->getSocket());
}

void            ASockManager::run( void )
{
    struct pollfd pfd;
    while (true)
    {
        std::vector<struct pollfd>  poll_fds;

        /* Setting all events for next poll */
        for (std::map<int, SockStream*>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); ++it)
        {
            pfd.fd = it->first;
            pfd.events = it->second->getPollEvents();
            pfd.revents = 0;
            poll_fds.push_back(pfd);
        }

        /* waiting events from fds */
        if (poll(poll_fds.data(), poll_fds.size(), 0) < 0)
        {
            std::cerr << "FATAL: poll error: " << strerror(errno) << std::endl;
            return ;
        }
        
        /* execute events handler */
        for (std::vector<struct pollfd>::reverse_iterator it = poll_fds.rbegin(); it != poll_fds.rend(); ++it)
        {
            if (it->revents != 0)
            {
                t_pollevent ev = this->_onPollEvent(it->fd, it->events);
                /* TODO if no more poll events are needed, delete ugly switch */
                switch (ev)
                {
                    case POLL_DELETE:
                        delete this->_sockets[it->fd];
		                this->_sockets.erase(it->fd);
                        poll_fds.erase( --(it.base()) );
                        break;
                }
            }
        }
    }
}
