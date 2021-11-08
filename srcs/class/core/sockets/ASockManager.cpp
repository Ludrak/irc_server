
#include "ASockManager.hpp"

ASockManager::ASockManager() : ASockHandler()
{}

void            ASockManager::delSocket(SockStream *sock)
{
    SockStream *s = this->_sockets[sock->getSocket()];
    if (s)
    {
        for (std::vector<pollfd>::iterator it = this->_poll_fds.begin(); it != this->_poll_fds.end(); it++)
            if (it->fd == sock->getSocket())
            {
                this->_poll_next_iterator = this->_poll_fds.erase(it);
                break ;
            }
        ASockHandler::delSocket(sock);
    }
}

void            ASockManager::run( void )
{
    struct pollfd pfd;
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
            std::cerr << "FATAL: poll error: " << strerror(errno) << std::endl;
            return ;
        }
        
        /* execute events handler */
        for (std::vector<struct pollfd>::iterator it = this->_poll_fds.begin(); it != this->_poll_fds.end(); ++it)
        {
            if (it->revents != 0)
            {
                this->_poll_next_iterator = it;
                this->_onPollEvent(it->fd, it->events);
                if (it == this->_poll_fds.end())
                {
                    it = this->_poll_next_iterator;
                    if (it == this->_poll_fds.end())
                        break;
                }
            }
        }
    }
}

