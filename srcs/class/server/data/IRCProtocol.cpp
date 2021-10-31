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
    size_t sz = data.find("\r\n");
    return (sz == std::string::npos ? 0 : sz + 2);
}

size_t      IRCProtocol::getMaximumPackageSize( void ) const
{
    return (255);
}
