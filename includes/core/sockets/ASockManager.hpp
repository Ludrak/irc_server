#ifndef ASOCKMANAGER_HPP
# define ASOCKMANAGER_HPP

#include "ASockHandler.hpp"

typedef int t_pollevent;
# define POLL_SUCCESS   0
# define POLL_NOTFOUND  1
# define POLL_DELETE    2
# define POLL_NOACCEPT  3


class ASockManager : public ASockHandler
{
	public:
		ASockManager(const std::string &ssl_cert_path="", const std::string &ssl_key_path="");

		virtual void        run(void);
		void                delSocket( const SockStream &sock);
	protected:
# ifdef POLL
		std::vector<pollfd>           _poll_fds;
# endif

		virtual t_pollevent _onPollEvent(int socket, int event) = 0;
		void				shutdown( void );
	
	protected:
		SSL_CTX			*_ssl_ctx;
		// Tell if using tls when the manager create a new socket
		//TODO put setter for _useTLS and call it in CommandConnect.cpp:67
		bool			_useTLS;
		bool			_running;
};


# endif // SOCK_MANAGER_HPP