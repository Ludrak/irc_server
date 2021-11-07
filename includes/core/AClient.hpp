#ifndef ACLIENT_HPP
# define ACLIENT_HPP

# include "ASockManager.hpp"
class AClient : public virtual ASockManager
{
	public:
		AClient(void);
		virtual ~AClient();

};

#endif /* ********************************************************* ACLIENT_H */