
#include "Protocol.hpp"

Protocol::Protocol()
{
}

Protocol::~Protocol()
{
}

size_t      Protocol::checkFormat( const std::string &data ) const
{
    return (data.size());
}

size_t      Protocol::getMaximumPackageSize ( void ) const
{
    return (10);
}

std::string Protocol::format( const std::string &data ) const
{
    return (data);
}

std::string Protocol::getData( const std::string &data ) const
{
    return (data);
}