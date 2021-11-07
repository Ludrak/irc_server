#include "AServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AServer::AServer(const std::string & host) : ASockManager()
{
	Logger::debug("Constructor AServer: " + host);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AServer::~AServer()
{
}

void						AServer::kick( SockStream &client )
{
	(void) client;
}

void						AServer::sendPackage( Package *pkg, SockStream &recipient)
{
	(void) pkg;
	(void) recipient;
}

/* ************************************************************************** */