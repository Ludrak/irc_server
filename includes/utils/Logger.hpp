#ifndef LOGGER_HPP
# define LOGGER_HPP

#include <string>
#include <iostream>

# define DEBUG      4
# define INFO       3
# define WARNING    2
# define ERROR      1

class Logger
{
    public:
        static void log (int level, const std::string &message);

    private:
        static void _debug( const std::string &message );
        static void _info( const std::string &message );
        static void _warning( const std::string &message );
        static void _error( const std::string &message );
};


#endif 