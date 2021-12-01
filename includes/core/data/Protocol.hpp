
#ifndef PROTOCOL_HPP
# define PROTOCOL_HPP

#include <iostream>
#include "IProtocol.hpp"

class Protocol : public IProtocol
{
    public:
        Protocol();
        virtual ~Protocol();
        virtual size_t      checkFormat( const std::string &data ) const;
        virtual size_t      getMaximumPackageSize ( void ) const;
        virtual std::string format( const std::string &data ) const;
        virtual std::string getData( const std::string &format ) const;
};

#endif // PROTOCOL_HPP