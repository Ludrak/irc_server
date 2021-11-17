#include "Server.hpp"

Server::Server(
    const UnRegisteredConnection &serv,
    const std::string &token,
    const std::string &name,
    const std::string &info,
    const std::string &host
) 
:   NetworkEntity(serv.getStream(), token, Server::value_type_forward),
    ServerInfo(info, serv.getVersion(), host)
{
    this->setPassword(serv.getPassword());
    this->setName(name);
    this->_family = SERVER_ENTITY_FAMILY;
    if (token.empty())
        Logger::critical ("registered forward server without uid");
}