#ifndef ACLIENT_HPP
# define ACLIENT_HPP

# include "ASockManager.hpp"
class AClient : public virtual ASockManager
{
	public:
		AClient(void);
		virtual ~AClient();

        bool            connectOn(const std::string host, const ushort port, IProtocol &protocol);

};

#endif /* ********************************************************* ACLIENT_H */