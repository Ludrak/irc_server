# include "ANode.hpp"

ANode::ANode(void) : ASockManager(), AServer(), AClient()
{
	std::cout << "ANode constructor" << std::endl;
}

t_pollevent					ANode::_onPollEvent(int socket, int event)
{
	t_pollevent pe = AServer::_onPollEvent(socket, event);
	if (pe == POLL_NOTFOUND)
		return AClient::_onPollEvent(socket, event);	
	return pe;
}
