#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <string>
# include <iostream>

# define RESET_ANSI		"\033[0m"		
# define RED_ANSI		"\033[91m"		
# define YELLOW_ANSI	"\033[93m"		
# define BLUE_ANSI		"\033[96m"		
# define LGREY_ANSI		"\033[37m"		
# define GREEN_ANSI		"\033[92m"		
# define DBLUE_ANSI		"\033[94m"		

# define DEBUG      4
# define INFO       3
# define WARNING    2
# define ERROR      1
# define CRITICAL	0

typedef unsigned int uint;

class Logger
{
    public:
        static uint		getLogLevel( void );
        static void		setLogLevel(uint level);
		
		static void 	log (int level, const std::string &message);
        static void		debug( const std::string &message );
        static void		info( const std::string &message );
        static void		warning( const std::string &message );
        static void		error( const std::string &message );
        static void		critical( const std::string &message );

    private:
		static uint logLevel;
};


#endif 