#ifndef ASOCKMANAGER_HPP
# define ASOCKMANAGER_HPP

#include "ASockHandler.hpp"

typedef int t_pollevent;
# define POLL_ERROR	   -1
# define POLL_SUCCESS   0
# define POLL_NOTFOUND  1
# define POLL_DELETE    2
# define POLL_NOACCEPT  3


class ASockManager : public ASockHandler
{
	public:
		ASockManager(const std::string &ssl_cert_path="", const std::string &ssl_key_path="");

		virtual void        run(void);
		bool				isOnTLS( void ) const;
		void                delSocket( const SockStream &sock);
	protected:
# ifdef POLL
		std::vector<pollfd>           _poll_fds;
# endif

		virtual t_pollevent _onPollEvent(int socket, int event) = 0;
		void				shutdown( void );

		void                initSSLContext(int type, const std::string &ssl_cert_path, const std::string &ssl_key_path);
	protected:
		SSL_CTX			*_ssl_serv_ctx;
		SSL_CTX			*_ssl_client_ctx;
		bool			_useTLS;
		bool			_running;
};


# endif // SOCK_MANAGER_HPP