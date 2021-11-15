#ifndef AENTITY_HPP
# define AENTITY_HPP

# include <iostream>
# include <string>
# include <list>
# include <map>
# include "StatusCode.hpp"

# define M_REGISTRATION_LIMITED		1 << 0

# define ENTITY_TYPE 							(1 << 0)
# define NETWORK_ENTITY_TYPE 					(1 << 1)
# define RELAYED_ENTITY_TYPE 					(1 << 2)
# define CLIENT_ENTITY_TYPE						(1 << 3)
# define SERVER_ENTITY_TYPE	        			(1 << 4)
# define FORWARD_SERVER_ENTITY_TYPE 			(1 << 5)
# define RELAYED_CLIENT_ENTITY_TYPE 			(1 << 6)
# define RELAYED_SERVER_ENTITY_TYPE				(1 << 7)
# define UNREGISTERED_CONNECTION_ENTITY_TYPE 	(1 << 8)

class	AEntity
{
	public:
		static const int	value_type;

		virtual ~AEntity();
		int					getType() const;
		const std::string	&getUID() const;
	
	protected:
		AEntity( const int type, const std::string &uid );

		int				_type;
		std::string		_uid;

};

const int AEntity::value_type = ENTITY_TYPE;

#endif /* ******************************************************** AENTITY_HPP */