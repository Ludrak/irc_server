#include "Client.hpp"

Client::Client(const UnRegisteredConnection &client)
: NetworkEntity(client.getStream(), Client::value_type), ClientInfo(client)
{
}