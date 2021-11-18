#include "Client.hpp"

//REVIEW see for handling 'username' see also in UserCommand
Client::Client(
    const IRCServer                 &server_reference,
    const UnRegisteredConnection    &client,
    const uint                      mode,
    const std::string               &real_name
)
:   NetworkEntity(client.getStream(), client.getUID(), Client::value_type),
    ClientInfo(real_name, mode, server_reference.getUID(), server_reference.getHostname())
{
    this->_name = client.getName();
    this->_password = client.getPassword();
    this->_family = CLIENT_ENTITY_FAMILY; //REVIEW  initialisation list
    if (this->_uid.empty())
        Logger::critical ("registered client without uid");
}

Client::~Client()
{

}

const std::list<Channel*>	&Client::getChannels() const
{
    return (this->_channels);
}


int 						Client::joinChannel(Channel &channel)
{
    if (!this->incrementJoinedChannels())
        return (1);
    uint ret = channel.addClient(*this);
	if (ret != SUCCESS)
		return ret;
    this->_channels.push_back(&channel);
	return SUCCESS;
}

void                        Client::leaveAllChannels(void)
{
    this->_channels = std::list<Channel*>();
}