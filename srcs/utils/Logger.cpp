#include "Logger.hpp"

uint		Logger::logLevel = DEBUG;
time_t 		Logger::_initTime;


void		Logger::init(uint level)
{
	setLogLevel(level);
	std::time(&Logger::_initTime);
	info("Logger initialisation done.");
}


time_t		Logger::getInitialTimestamp( void )
{
	return Logger::_initTime;
}

std::string	Logger::getTimestamp( void )
{
	char				buffer[80];
	struct tm*			timeinfo;
    struct timeval		time_now;
	std::stringstream	ss;

    gettimeofday(&time_now, nullptr);
	timeinfo = localtime(&time_now.tv_sec);
	strftime(buffer,sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
	std::string ms(ntos(time_now.tv_usec));
	ms.resize(3);
	ss << "["TIMESTAMP_COLOR << buffer << ":" << ms << RESET_ANSI"]";
	return ss.str();

}



void		Logger::log( uint level, const std::string &message )
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
		case DEBUG:
            Logger::debug(message);
            break;
        default:
            Logger::core(message);
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

void	Logger::core( const std::string &message )
{
	if (Logger::logLevel >= CORE )
	    std::cout << Logger::getTimestamp() << CORE_LOG << message << RESET_ANSI"\033[23m" << std::endl;
}

void	Logger::debug( const std::string &message )
{
	if (Logger::logLevel >= DEBUG )
	    std::cout << Logger::getTimestamp() << DEBUG_LOG << message << RESET_ANSI"\033[23m" << std::endl;
}

void	Logger::info( const std::string &message )
{
	if (Logger::logLevel >= INFO )
    	std::cout << Logger::getTimestamp() << INFO_LOG << message  << RESET_ANSI << std::endl;
}

void	Logger::warning( const std::string &message )
{
	if (Logger::logLevel >= WARNING )
 	   std::cerr << Logger::getTimestamp() << WARNING_LOG << message << RESET_ANSI"\033[23m" << std::endl;
}

void	Logger::error( const std::string &message )
{
	if (Logger::logLevel >= ERROR )
    	std::cerr << Logger::getTimestamp() << ERROR_LOG << message << RESET_ANSI << std::endl;
}

void    Logger::critical( const std::string &message )
{
	if (Logger::logLevel >= CRITICAL )
	    std::cerr << Logger::getTimestamp() << CRITICAL_LOG << message << RESET_ANSI << std::endl;
}
