# include "ANode.hpp"

ANode::ANode(const std::string & host, const std::string &ssl_cert_path, const std::string &ssl_key_path) 
: ASockManager(ssl_cert_path, ssl_key_path), AServer(host), AClient()
{
	Logger::debug("ANode constructor");
}

ANode::~ANode(void)
{
	Logger::debug("ANode destructor");
}

t_pollevent					ANode::_onPollEvent(int socket, int event)
{
	t_pollevent pe = AServer::_onPollEvent(socket, event);
	if (pe == POLL_NOTFOUND)
		return AClient::_onPollEvent(socket, event);
	return pe;
}
