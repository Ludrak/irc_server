#include "Server.hpp"

Server::Server(
    UnRegisteredConnection &serv,
    const std::string   &token,
    const std::string   &name,
    const std::string   &info,
    const std::string   &host,
    const int           type
) 
:   NetworkEntity(serv.getStream(), token, type),
    ServerInfo(info, serv.getVersion(), host, serv.getFlags())
{
    this->setPassword(serv.getPassword());
    this->setName(name);
    this->_family = SERVER_ENTITY_FAMILY;
    if (token.empty())
        Logger::critical ("registered forward server without uid");
}