#include "ASockManager.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ASockManager::ASockManager(int sock_param)
{
	(void) sock_param;
	std::cout << "Constructor ASockManager" << std::endl;
}

/*
** ------------------------------- DESTRUCTOR --------------------------------
*/

ASockManager::~ASockManager()
{
	std::cout << "destructor ASockManager" << std::endl;
}

/* ************************************************************************** */