#include "CommandNick.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandNick::CommandNick()
{
}

CommandNick::CommandNick( const CommandNick & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandNick::~CommandNick()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

CommandNick &				CommandNick::operator=( CommandNick const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

uint					CommandNick::operator()(AEntity & executor, std::string params)
{
	(void) executor;
	(void) params;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandPass::hasPermissions(AEntity & executor)
{
	//TODO implement right for NICK
	( void) executor;
	return SUCCESS;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */