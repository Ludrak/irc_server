#include "Client.hpp"

Client::Client(const UnRegisteredConnection &client)
: NetworkEntity(client.getStream(), client.getUID(), Client::value_type), ClientInfo(client)
{
    if (this->_uid.empty())
        Logger::critical ("registered client without uid");
}