
#ifndef PROTOCOL_HPP
# define PROTOCOL_HPP

#include <string>
#include <iostream>
#include "IProtocol.hpp"

class Protocol : public IProtocol
{
    public:
        Protocol()
        {}

        virtual ~Protocol()
        {}

        virtual size_t      isProtocol( const std::string &data ) const
        {
            std::cout << "APROTOCOL" << std::endl;
            return (data.size());
        }

        virtual size_t      getMaximumPackageSize ( void ) const
        {
            return (10);
        }

        // virtual Protocol   *clone() const
        // {
        //     return (new Protocol());
        // }
};

#endif // PROTOCOL_HPP