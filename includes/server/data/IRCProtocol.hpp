#ifndef IRCPROTOCOL_HPP
# define IRCPROTOCOL_HPP

#include "Protocol.hpp"

class IRCProtocol : public Protocol
{
    public:
        static bool    isProtocol( const std::string &data );
        static size_t  getMaximumPackageSize ( void );
};

#endif // IRC_PROTOCOL_HPP