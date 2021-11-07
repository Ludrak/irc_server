
#include <string>

#ifndef IPROTOCOL_HPP
# define IPROTOCOL_HPP

class IProtocol
{
    public:
        virtual ~IProtocol() {}
        virtual size_t      checkFormat( const std::string &data ) const = 0;
        virtual size_t      getMaximumPackageSize ( void ) const = 0;
        virtual std::string format( const std::string &data ) const = 0;
        virtual std::string getData( const std::string &format ) const = 0;
};

#endif // IPROTOCOL_HPP