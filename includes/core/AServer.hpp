#ifndef ASERVER_HPP
# define ASERVER_HPP

# include "ASockManager.hpp"
class AServer : public virtual ASockManager
{
	public:
		AServer(int param);
		virtual ~AServer();
		void				kick( SockStream &client );
		void				sendPackage( Package *pkg, SockStream &recipient);

};

#endif /* ********************************************************* ASERVER_H */