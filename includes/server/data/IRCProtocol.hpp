#ifndef IRCPROTOCOL_HPP
# define IRCPROTOCOL_HPP

#include "Protocol.hpp"

class IRCProtocol : public IProtocol
{
    public:
        IRCProtocol();
        virtual     ~IRCProtocol();
        size_t      isProtocol( const std::string &data ) const;
        size_t      getMaximumPackageSize ( void ) const;
        //Protocol    *clone() const;
};

#endif // IRC_PROTOCOL_HPP