
#include <string>

#ifndef IPROTOCOL_HPP
# define IPROTOCOL_HPP

class IProtocol
{
    public:
        virtual ~IProtocol() {}
        virtual size_t      isProtocol( const std::string &data ) const = 0;
        virtual size_t      getMaximumPackageSize ( void ) const = 0;
};

#endif // IPROTOCOL_HPP