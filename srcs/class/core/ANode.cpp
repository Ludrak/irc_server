# include "ANode.hpp"

ANode::ANode(int someParam) : ASockManager(someParam), AServer(someParam), AClient(someParam)
{
	std::cout << "ANode constructor" << std::endl;
}