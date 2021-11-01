# client command handle
    void    command(std::vector<std::string> params)
    { }

# irc clients representation idea
```cpp
/*
** using clients as st::map
*/
std::map<SockStream, AEntity>   clients;

/*
** AEntity is now only an information interface
** of client / channel / ... 
**
** that way clients doesn't contain a proper socket
** but the server has one to communicate with it.
** 
** it also means that on _onClient*(SockStream &ss) events
** it will be possible to do such things :
*/
Client c = ss.getServer().getClients().at(ss);
/*     ^ 
**     will be the client interface that we want
**     and we'll be sure that ss is the corresponding
**     key of that client
*/

/*
** therefore adding clients to the server should be done this way:
*/
ss.getServer().getClients().insert(std::make_pair(ss, Client(...)))

/*
** which could be simplified on server side by adding
*/
IRCServer::addClient(SockStream &ss, Client &client_interface)
{
    this->_clients.insert(std::make_pair(ss, client_interface));
}

/*
** and so doing that in AServer::_onClientJoin(SockStream &ss)
** to add a new client to the server:
*/
IRCServer &server = ss.getServer();
server.addClient(ss, Client(...));
```