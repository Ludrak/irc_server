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
	//REVIEW optimisable par sauvegarde de valeur (par exemple recalculer la string des heures seulement lorsque les minutes reviennent à 0) (à creuser)
	double	time = std::difftime(std::time(NULL), Logger::_initTime);
	uint	seconds = static_cast<uint>(time);
	uint	minutes = (seconds / 60) % 60;
	uint	hours = (seconds / 3600) % 24;
	uint	days = (seconds / 86400);
	seconds = seconds % 60;
	std::stringstream ss;
	ss << std::setfill('0') << "[" << days << "D" << std::setw(2) << hours << "h" << std::setw(2) << minutes << ":" << std::setw(2) << seconds << "]";
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

void    Logger::core( const std::string &message )
{
	if (Logger::logLevel >= CORE )
	    std::cout << Logger::getTimestamp() << "[" << DBLUE_ANSI << "CORE" << RESET_ANSI << "]   | " << message << std::endl;
}

void    Logger::debug( const std::string &message )
{
	if (Logger::logLevel >= DEBUG )
	    std::cout << Logger::getTimestamp() << "[" << LGREY_ANSI << "DEBUG" << RESET_ANSI << "]  | " << message << std::endl;
}

void    Logger::info( const std::string &message )
{
	if (Logger::logLevel >= INFO )
    	std::cout << Logger::getTimestamp() << "[" << GREEN_ANSI << "INFO" << RESET_ANSI << "]   | " << message << std::endl;
}

void    Logger::warning( const std::string &message )
{
	if (Logger::logLevel >= WARNING )
 	   std::cerr << Logger::getTimestamp() << "[" << YELLOW_ANSI << "WARNING" << RESET_ANSI << "]| " << YELLOW_ANSI << message << RESET_ANSI << std::endl;
}

void    Logger::error( const std::string &message )
{
	if (Logger::logLevel >= ERROR )
    	std::cerr << Logger::getTimestamp() << "[" << RED_ANSI << "ERROR" << RESET_ANSI << "]  | " << RED_ANSI << message << RESET_ANSI << std::endl;
}

void    Logger::critical( const std::string &message )
{
	if (Logger::logLevel >= CRITICAL )
	    std::cerr << Logger::getTimestamp() << RED_ANSI << "\033[48;2;255;165;0m[" << DBLUE_ANSI << "CRITICAL" << RED_ANSI << "]" << RESET_ANSI << " " << "\033[38;2;255;165;0m" << message << RESET_ANSI << std::endl;
}
