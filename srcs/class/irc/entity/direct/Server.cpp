#include "Server.hpp"

Server::Server(const UnRegisteredConnection &serv)
: NetworkEntity(serv.getStream(), Server::value_type), ServerInfo(serv)
{
}

Server::Server(SockStream& sock, std::string uid, std::string name, std::string info) 
: NetworkEntity(sock, Server::value_type_forward), ServerInfo(uid, name, info)
{
}