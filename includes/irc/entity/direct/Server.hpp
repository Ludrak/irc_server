
#ifndef SERVER_HPP
# define SERVER_HPP

#include "NetworkEntity.hpp"
#include "ServerInfo.hpp"
#include "UnRegisteredConnection.hpp"

# define SERVER_ENTITY_TYPE	        4
# define FORWARD_SERVER_ENTITY_TYPE 5

class Server : public NetworkEntity, public ServerInfo
{
	public:
		static const int	value_type;
        static const int	value_type_forward;
	
	public:
		/* initalizes from an unregistred connection */
		/* type is set to SERVER_ENTITY_TYPE */
		Server (const UnRegisteredConnection &server);

		/* server is  build by SockStream& and by giving all its common informations */
		/* type is set to FORWARD_SERVER_ENTITY_TYPE */
		Server (SockStream &sock, const std::string &uid, const std::string &name, const std::string &info, const std::string &pass);
};

const int Server::value_type = SERVER_ENTITY_TYPE;
const int Server::value_type_forward = FORWARD_SERVER_ENTITY_TYPE;

#endif // SERVER_HPP