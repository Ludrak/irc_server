# client command handle
    void    command(std::vector<std::string> params)
    { }

# SHA snippet

    #include <iostream>
    #include <iomanip>
    #include <sstream>
    #include <string>

    using namespace std;

    #include <openssl/sha.h>
    string sha256(const string str)
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, str.c_str(), str.size());
        SHA256_Final(hash, &sha256);
        stringstream ss;
        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            ss << hex << setw(2) << setfill('0') << (int)hash[i];
        }
        return ss.str();
    }

    int main() {
        cout << sha256("1234567890_1") << endl;
        cout << sha256("1234567890_2") << endl;
        cout << sha256("1234567890_3") << endl;
        cout << sha256("1234567890_4") << endl;
        return 0;
    }
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
