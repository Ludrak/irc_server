#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <string>
# include <iostream>
# include <ctime>
# include <iomanip>
# include <sstream>
# include <sys/time.h>
# include "ntos.hpp"

// define basic ansi colors
# define RESET_ANSI		"\033[0m"		
# define BOLD_ANSI		"\033[1m"		
# define RED_ANSI		"\033[91m"		
# define YELLOW_ANSI	"\033[93m"		
# define BLUE_ANSI		"\033[96m"		
# define LGREY_ANSI		"\033[37m"		
# define GREEN_ANSI		"\033[92m"		
# define DBLUE_ANSI		"\033[94m"		

// define prefix id's 
# define CORE		    5
# define DEBUG          4
# define INFO           3
# define WARNING        2
# define ERROR          1
# define CRITICAL	    0

// define prefixes RGB's here
# define CORE_COLOR		"\033[38;2;240;240;220m"
# define DEBUG_COLOR    "\033[38;2;180;210;255m\033[3m"
# define INFO_COLOR     "\033[38;2;150;250;105m"
# define WARNING_COLOR  "\033[38;2;250;230;15m\033[3m"
# define ERROR_COLOR    "\033[38;2;255;67;33m\033[1m"
# define CRITICAL_COLOR	"\033[48;2;250;100;20m\033[1m"

// define timestamp color here
# define TIMESTAMP_COLOR    "\033[38;2;100;100;120m"

// define message output color here
# define CORE_MSG_COLOR     "\033[38;2;210;210;190m"
# define DEBUG_MSG_COLOR    "\033[38;2;180;210;255m"
# define INFO_MSG_COLOR     "\033[38;2;230;230;210m"
# define WARNING_MSG_COLOR  "\033[38;2;250;230;40m"
# define ERROR_MSG_COLOR    "\033[38;2;255;77;50m"
# define CRITICAL_MSG_COLOR "\033[38;2;250;190;80m"

// define prefix structure here
# define CORE_LOG		"["CORE_COLOR"CORE"RESET_ANSI"] | "CORE_MSG_COLOR"\033[3m"
# define DEBUG_LOG      "["DEBUG_COLOR"DEBUG"RESET_ANSI"]| "DEBUG_MSG_COLOR"\033[3m"
# define INFO_LOG       "["INFO_COLOR"INFO"RESET_ANSI"] | "INFO_MSG_COLOR
# define WARNING_LOG    "["WARNING_COLOR"WARN"RESET_ANSI"] | "WARNING_MSG_COLOR"\033[3m"
# define ERROR_LOG      "["ERROR_COLOR"ERROR"RESET_ANSI"]| "ERROR_MSG_COLOR
# define CRITICAL_LOG	"["CRITICAL_COLOR"CRITICAL"RESET_ANSI"]| "CRITICAL_MSG_COLOR

typedef unsigned int uint;

class Logger
{
    public:
		static void			init( uint level);

		static time_t		getInitialTimestamp( void );
		static std::string	getTimestamp( void );
        static uint			getLogLevel( void );
        static void			setLogLevel(uint level);

		static void			log(uint level, const std::string &message);
        static void			core( const std::string &message );
        static void			debug( const std::string &message );
        static void			info( const std::string &message );
        static void			warning( const std::string &message );
        static void			error( const std::string &message );
        static void			critical( const std::string &message );

    private:
		static time_t		_initTime;
		static uint			logLevel;

};


#endif 