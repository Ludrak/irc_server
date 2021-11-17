#include "Client.hpp"

Client::Client(const UnRegisteredConnection &client)
: NetworkEntity(client.getStream(), client.getUID(), Client::value_type), ClientInfo(client)
{
    this->_family = CLIENT_ENTITY_FAMILY;
    if (this->_uid.empty())
        Logger::critical ("registered client without uid");
}

Client::~Client()
{

}