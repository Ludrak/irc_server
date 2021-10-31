
#include <string>

class IProtocol
{
    public:
        virtual ~IProtocol() {}
        virtual size_t      isProtocol( const std::string &data ) const = 0;
        virtual size_t      getMaximumPackageSize ( void ) const = 0;
};