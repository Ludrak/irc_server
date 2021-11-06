#ifndef ASockManager_HPP
# define ASockManager_HPP

# include <iostream>
# include <string>
# include "SockStream.hpp"

class ASockManager
{
	public:

		ASockManager(int sock_param);
		virtual ~ASockManager();

		void	run(void)
		{
			;
		}

};

#endif /* ***************************************************** ASockManager_H */