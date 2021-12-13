#ifndef ACLIENT_HPP
# define ACLIENT_HPP

class AClient;

# include <iostream>
# include <string>
# include <vector>
# include <poll.h>
# include <map>
# include "SockStream.hpp"
# include "ASockManager.hpp"

class AClient : public virtual ASockManager
{
    public:
    class ConnectionException : std::exception
    {
		//REVIEW remove usage of this exception? (no exceptions used in AServer)
        public:
            virtual const char  *what() const throw()
            {
                return ("can't connect to server");
            }
    };

    public:
        AClient(const std::string &ssl_cert_path="", const std::string & ss_key_path="");
        virtual ~AClient();

        bool        	   		connectOn(const std::string host, const ushort port, const IProtocol &protocol, const bool useTLS=false);
        void					sendServerPackage( Package *package, SockStream &server_recipient);
    
    protected:
        virtual t_pollevent     _onPollEvent(int socket, int event);
        virtual void			_onConnect ( SockStream &server) = 0;
        virtual void    		_onRecv( SockStream &server,  Package &pkg) = 0;
        virtual void    		_onQuit( SockStream &server) = 0;

    private:

        t_pollevent				_pollFromServers(SockStream & sock, int event);
        t_pollevent				_pollInClients(SockStream & sock);
		t_pollevent				_pollOutClients(SockStream & sock);
};

#endif // ACLIENT_HPP
