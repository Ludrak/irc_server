#include "Logger.hpp"

void    Logger::log( int level, const std::string &message )
{
    switch (level)
    {
        case DEBUG:
            Logger::_debug(message);
            break;
        case WARNING:
            Logger::_warning(message);
            break;
        case ERROR:
            Logger::_error(message);
            break;
        default:
            Logger::_info(message);
            break;
    }
}

void    Logger::_debug( const std::string &message )
{
    std::cout << "[DEBUG] " << message << std::endl;
}

void    Logger::_info( const std::string &message )
{
    std::cout << "[INFO] " << message << std::endl;
}

void    Logger::_warning( const std::string &message )
{
    std::cout << "[WARNING] " << message << std::endl;
}

void    Logger::_error( const std::string &message )
{
    std::cout << "[ERROR] " << message << std::endl;
}
