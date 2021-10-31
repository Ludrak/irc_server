
#ifndef PROTOCOL_HPP
# define PROTOCOL_HPP

#include <string>
#include <iostream>
#include "IProtocol.hpp"

class Protocol : public IProtocol
{
    public:
        Protocol();
        virtual ~Protocol();
        virtual size_t      isProtocol( const std::string &data ) const;
        virtual size_t      getMaximumPackageSize ( void ) const;
};

#endif // PROTOCOL_HPP