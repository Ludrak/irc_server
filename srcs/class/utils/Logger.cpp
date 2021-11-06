#include "Logger.hpp"

uint 	Logger::logLevel = DEBUG;

void    Logger::log( int level, const std::string &message )
{
    switch (level)
    {
        case CRITICAL:
            Logger::critical(message);
            break;
        case ERROR:
            Logger::error(message);
            break;
		case WARNING:
            Logger::warning(message);
            break;
        case INFO:
            Logger::info(message);
            break;
        default:
            Logger::debug(message);
            break;
    }
}

uint	Logger::getLogLevel( void )
{
	return Logger::logLevel;
}

void	Logger::setLogLevel(uint level)
{
	Logger::logLevel = level;
}

void    Logger::debug( const std::string &message )
{
	if (Logger::logLevel >= DEBUG )
	    std::cout << "[" << LGREY_ANSI << "DEBUG" << RESET_ANSI << "]    |  " << message << std::endl;
}

void    Logger::info( const std::string &message )
{
	if (Logger::logLevel >= INFO )
    	std::cout << "[" << GREEN_ANSI << "INFO" << RESET_ANSI << "]     |  " << message << std::endl;
}

void    Logger::warning( const std::string &message )
{
	if (Logger::logLevel >= WARNING )
 	   std::cerr << "[" << YELLOW_ANSI << "WARNING" << RESET_ANSI << "]  |  " << YELLOW_ANSI << message << RESET_ANSI << std::endl;
}

void    Logger::error( const std::string &message )
{
	if (Logger::logLevel >= ERROR )
    	std::cerr << "[" << RED_ANSI << "ERROR" << RESET_ANSI << "]    |  " << RED_ANSI << message << RESET_ANSI << std::endl;
}

void    Logger::critical( const std::string &message )
{
	if (Logger::logLevel >= CRITICAL )
	    std::cerr << RED_ANSI << "\033[48;2;255;165;0m[" << DBLUE_ANSI << "CRITICAL" << RED_ANSI << "]" << RESET_ANSI << " |  " << "\033[38;2;255;165;0m" << message << RESET_ANSI << std::endl;
}
