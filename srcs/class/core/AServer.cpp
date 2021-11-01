#include "AServer.hpp"

/*
** -------------------------------- STATICS --------------------------------
*/

uint		AServer::_default_max_connections = 50;


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AServer::AServer( IProtocol & protocol, const std::string &host, int port) : SockStream(host, port, protocol), _max_connection(AServer::_default_max_connections)
{
	this->_init_server();
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AServer::~AServer()
{
	for (std::map<int, SockStream*>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		delete (*it).second;
	}
}

/*
** --------------------------------- METHODS ----------------------------------
*/
bool						AServer::run( void )
{
	std::vector<struct pollfd>	poll_fds;
	struct pollfd 				new_pfd = {.fd = this->_socket, .events = POLLIN, .revents = 0};
	poll_fds.push_back(new_pfd);

	if (listen(this->_socket, this->_max_connection) != 0)
		throw AServer::ListenException();
	while (1)
	{
		for (std::vector<struct pollfd>::iterator it = poll_fds.begin() + 1; it != poll_fds.end(); it++)
		{
			it->events = this->_clients[it->fd]->getPollEvents();
		}

		if (poll(poll_fds.data(), poll_fds.size(), -1) == -1)
			throw AServer::PollException();
		if (poll_fds.at(0).revents & POLLIN)
		{
			try {
				new_pfd.fd = this->_acceptConnection().getSocket();
				new_pfd.events = POLLIN; 
				new_pfd.revents = 0;
				poll_fds.push_back(new_pfd);
			}
			catch (const AServer::IncomingConnectionException &e)
			{
				std::cerr << "[FT_IRC]: " << e.what() << "\n"; 
			}
			poll_fds.at(0).revents = 0;
		}
		std::vector<struct pollfd>::reverse_iterator it = poll_fds.rbegin();
		for (; it != poll_fds.rend(); it++)
		{	
			if (it->revents & POLLOUT && !this->_clients[it->fd]->getPendingData().empty())
			{
				Package	&current_pkg = **this->_clients[it->fd]->getPendingData().begin();
				int recipient_sock = current_pkg.getRecipient()->getSocket();

				char data_buffer[SEND_BUFFER_SZ] = { 0 };
				size_t	data_sz = current_pkg.getData().size() > SEND_BUFFER_SZ ? SEND_BUFFER_SZ : current_pkg.getData().size();
				std::memcpy(data_buffer, current_pkg.getData().c_str(), data_sz);

				size_t byte_size = send(recipient_sock, data_buffer, data_sz, 0);
				current_pkg.nflush(byte_size);

				if (current_pkg.isInvalid() || current_pkg.getData().empty())
				{
					delete &current_pkg;
					this->_clients[it->fd]->getPendingData().remove(&current_pkg);
					if (this->_clients[it->fd]->getPendingData().size() == 0)
						this->_clients[it->fd]->delPollEvent(POLLOUT);
				}
			}
			if (it->revents & POLLIN)
			{
				std::vector<char> buf(RECV_BUFFER_SZ);
				size_t byte_size = recv(it->fd, reinterpret_cast<void *>(buf.data()), RECV_BUFFER_SZ, MSG_DONTWAIT);
				if (byte_size < 0) 
				{
					std::cerr << "error from client in socket " << this->_clients[it->fd]->getSocket() << std::endl;
					continue ;
				}
				else if (byte_size == 0)
				{
					this->_onClientQuit(*this->_clients[it->fd]);
					delete this->_clients[it->fd];
					this->_clients.erase(it->fd);
					poll_fds.erase( --(it.base()) );
					continue ;
				}
				this->_clients[it->fd]->getRecievedData().addData(std::string (buf.begin(), buf.begin() + byte_size));
				if (this->_clients[it->fd]->getRecievedData().isInvalid())
					continue;
				while (!this->_clients[it->fd]->getRecievedData().isInvalid()){
					this->_onClientRecv(*this->_clients[it->fd], this->_clients[it->fd]->getRecievedData());
					this->_clients[it->fd]->getRecievedData().flush();
				}
				it->revents = 0;
			}
		}
	}
	return true;
}

SockStream					&AServer::_acceptConnection()
{	
	sockaddr_in         cli_addr;
	int					socket_client;
	socklen_t			cli_len = sizeof(cli_addr);

	socket_client = accept(this->_socket, reinterpret_cast<struct sockaddr *>(&cli_addr), &cli_len);
	if (socket_client < 0)
		throw AServer::IncomingConnectionException();

	SockStream *newSock = new SockStream(this, socket_client, cli_addr, *this->_protocol);
	std::pair<int, SockStream *> p = std::make_pair(socket_client, newSock);
	this->_clients.insert(p);
	this->_onClientJoin(*newSock);
	return *newSock;
}

bool						AServer::_init_server( void )
{
	if (bind(this->_socket, reinterpret_cast<sockaddr *>(&this->_addr), sizeof(this->_addr)) != 0)
		throw AServer::AddressBindException();
	return true;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::map<int, SockStream*>	&AServer::getClients()
{
	return (this->_clients);
}

uint						AServer::getMaxConnection( void ) const
{
	return this->_max_connection;
}

void						AServer::setMaxConnection( uint nb)
{
	this->_max_connection = nb;
}

/* ************************************************************************** */