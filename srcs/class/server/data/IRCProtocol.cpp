#include "IRCProtocol.hpp"

#include <iostream>

IRCProtocol::IRCProtocol() 
{
}

IRCProtocol::~IRCProtocol()
{
}

size_t      IRCProtocol::isProtocol( const std::string &data ) const
{
    // find \r\n in data and return the size of the package
    //if (data.size() >= 1 && (data.back() != '\n'))
    //    return false;
    size_t sz = data.find("\n") + 1;
    return (sz == std::string::npos ? 0 : sz);
}

size_t      IRCProtocol::getMaximumPackageSize( void ) const
{
    return (255);
}

// Protocol    *IRCProtocol::clone() const
// {
//     return (new IRCProtocol());
// }