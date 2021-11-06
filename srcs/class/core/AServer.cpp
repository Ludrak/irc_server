#include "AServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AServer::AServer(int param) : ASockManager(param)
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