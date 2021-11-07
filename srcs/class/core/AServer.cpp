#include "AServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AServer::AServer(void) : ASockManager()
{
	std::cout << "Constructor AServer" << std::endl;
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