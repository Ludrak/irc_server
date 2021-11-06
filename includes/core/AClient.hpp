#ifndef ACLIENT_HPP
# define ACLIENT_HPP

# include "ASockManager.hpp"
class AClient : public virtual ASockManager
{
	public:
		AClient(int param);
		virtual ~AClient();

};

#endif /* ********************************************************* ACLIENT_H */