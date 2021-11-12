
#include "ASockManager.hpp"

ASockManager::ASockManager() : ASockHandler()
{
	Logger::debug("ASockManager constructor");
}

void            ASockManager::delSocket(const SockStream &sock)
{
	for (std::vector<pollfd>::iterator it = this->_poll_fds.begin(); it != this->_poll_fds.end(); it++)
	{
		if (it->fd == sock.getSocket())
		{
			this->_poll_next_iterator = this->_poll_fds.erase(it);
			break ;
		}
	}
	ASockHandler::delSocket(sock);
}

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
        
        /* execute events handler */
        for (std::vector<struct pollfd>::iterator it = this->_poll_fds.begin(); it != this->_poll_fds.end(); ++it)
        {
            if (it->revents != 0)
            {
                this->_poll_next_iterator = it;
				size_t sz = this->_sockets.size();
                Logger::debug("Polling socket " + std::to_string(it->fd));
                this->_onPollEvent(it->fd, it->revents);
            	it = this->_poll_next_iterator;
                if (this->_sockets.size() != sz)
                {
                    break;
                }
            }
        }
    }
}

