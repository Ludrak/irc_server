#include "IRCProtocol.hpp"

bool    IRCProtocol::isProtocol( const std::string &data )
{
    if (data.size() >= 2 && (data.back() != '\n'))
        return false;
    return (true);
}

size_t  IRCProtocol::getMaximumPackageSize( void )
{
    return (255);
}