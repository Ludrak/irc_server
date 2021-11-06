#include "AClient.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AClient::AClient(int param) : ASockManager(param)
{
	std::cout << "Constructor AClient" << std::endl;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AClient::~AClient()
{
}


/* ************************************************************************** */