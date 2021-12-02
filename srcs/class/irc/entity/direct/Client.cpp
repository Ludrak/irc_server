#include "Client.hpp"

//REVIEW see for handling 'username' see also in UserCommand
Client::Client(
    IRCServer                       &server_reference,
    UnRegisteredConnection          &client,
    const uint                      mode,
    const std::string               &real_name
)
:   NetworkEntity(client.getStream(), client.getUID(), Client::value_type),
    ClientInfo(server_reference, real_name, mode, server_reference.getUID(), server_reference.getHostname())
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

const std::string			Client::getIdent( void ) const
{
	return (this->_uid + "!" + this->_name + "@" + this->_host);
}

const std::string			Client::getPrefix( void ) const
{
	return ":" + this->getIdent() + " ";
}