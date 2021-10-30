
#ifndef PROTOCOL_HPP
# define PROTOCOL_HPP

#include <string>

class Protocol
{
    public:

        static bool    isProtocol( const std::string &data )
        {
            (void) data;
            return (false);
        };

        static size_t  getMaximumPackageSize ( void )
        {
            return (1000);
        }
};

#endif // PROTOCOL_HPP