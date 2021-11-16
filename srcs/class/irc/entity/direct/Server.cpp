#include "Server.hpp"

Server::Server(const UnRegisteredConnection &serv)
: NetworkEntity(serv.getStream(), serv.getUID(), Server::value_type), ServerInfo(serv)
{
    this->_family = SERVER_ENTITY_FAMILY;
    if (this->_uid.empty())
        Logger::critical ("registered server without uid");
}

Server::Server(SockStream& sock, const std::string &uid, const std::string &name, const std::string &info, const std::string &pass) 
: NetworkEntity(sock, uid, Server::value_type_forward), ServerInfo(name, info, pass)
{
    this->_family = SERVER_ENTITY_FAMILY;
    if (this->_uid.empty())
        Logger::critical ("registered forward server without uid");
}