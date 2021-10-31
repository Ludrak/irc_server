
#include "Protocol.hpp"

Protocol::Protocol()
{
}

Protocol::~Protocol()
{
}

size_t      Protocol::isProtocol( const std::string &data ) const
{
    return (data.size());
}

size_t      Protocol::getMaximumPackageSize ( void ) const
{
    return (10);
}