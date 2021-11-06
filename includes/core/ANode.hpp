#ifndef ANODE_HPP
#define ANODE_HPP

# include "AServer.hpp"
# include "AClient.hpp"

class ANode : public AServer, public AClient
{
	protected:
		ANode(int someParam);

		virtual void			_onClientJoin(SockStream &s) = 0;
		virtual void			_onClientRecv(SockStream &s, Package &pkg) = 0;
		virtual void			_onClientQuit(SockStream &s) = 0;
		void					_onPollEvent(SockStream &s);

};

#endif 
