#include "Logger.hpp"

uint			Logger::_logLevel = DEBUG;
struct timeval 	Logger::_initTime;
std::ofstream	Logger::_logfile;


void		Logger::init(uint level, const std::string & filename)
{
	setLogLevel(level);
    gettimeofday(&_initTime, nullptr);
	initLogfile(filename);
	info("Logger initialisation done.");
}

void		Logger::initLogfile(const std::string & filename )
{
	if (!filename.empty())
	{
		/* set a log file */
		_logfile.open(filename, std::ofstream::out | std::ofstream::app);
		if (_logfile.is_open() == false)
			throw Logger::logFileException();
		info("logfile (" + filename + ") initialisation done");
	}
}
const struct timeval	&Logger::getInitialTimestamp( void )
{
	return Logger::_initTime;
}

std::string		Logger::timeToString( const struct timeval & time )
{
	char		buffer[80];
	struct tm*	timeinfo;

	timeinfo = localtime(&time.tv_sec);
	buffer[strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo)] = '\0';
	return buffer;
}

std::string	Logger::getTimestamp( bool color )
{
    struct timeval		time_now;
	std::stringstream	ss;

    gettimeofday(&time_now, nullptr);

	std::string ms(ntos(time_now.tv_usec));
	ms.resize(3);
	if (color)
		ss << "["TIMESTAMP_COLOR << timeToString(time_now) << ":" << ms << RESET_ANSI"]";
	else
		ss << "[" << timeToString(time_now) << ":" << ms << "]";
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
	return Logger::_logLevel;
}

void	Logger::setLogLevel(uint level)
{
	Logger::_logLevel = level;
}

void	Logger::core( const std::string &message )
{
	Logger::logToFile(message);
	if (Logger::_logLevel >= CORE )
	    std::cout << Logger::getTimestamp() << CORE_LOG << message << RESET_ANSI"\033[23m" << std::endl;
}

void	Logger::debug( const std::string &message )
{
	Logger::logToFile(message);
	if (Logger::_logLevel >= DEBUG )
	    std::cout << Logger::getTimestamp() << DEBUG_LOG << message << RESET_ANSI"\033[23m" << std::endl;
}

void	Logger::info( const std::string &message )
{
	Logger::logToFile(message);
	if (Logger::_logLevel >= INFO )
    	std::cout << Logger::getTimestamp() << INFO_LOG << message  << RESET_ANSI << std::endl;
}

void	Logger::warning( const std::string &message )
{
	Logger::logToFile(message);
	if (Logger::_logLevel >= WARNING )
 	   std::cerr << Logger::getTimestamp() << WARNING_LOG << message << RESET_ANSI"\033[23m" << std::endl;
}

void	Logger::error( const std::string &message )
{
	Logger::logToFile(message);
	if (Logger::_logLevel >= ERROR )
    	std::cerr << Logger::getTimestamp() << ERROR_LOG << message << RESET_ANSI << std::endl;
}

void    Logger::critical( const std::string &message )
{
	Logger::logToFile(message);
	if (Logger::_logLevel >= CRITICAL )
	    std::cerr << Logger::getTimestamp() << CRITICAL_LOG << message << RESET_ANSI << std::endl;
}


inline void	Logger::logToFile(const std::string & message)
{
	Logger::_logfile << Logger::getTimestamp(false) << " " << message << std::endl;
}