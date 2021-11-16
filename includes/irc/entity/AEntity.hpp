#ifndef AENTITY_HPP
# define AENTITY_HPP

# include <iostream>
# include <string>
# include <list>
# include <map>
# include "status.hpp"

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
# define CHANNEL_ENTITY_TYPE					(1 << 9)

# define ENTITY_FAMILY							0
# define CLIENT_ENTITY_FAMILY					1
# define SERVER_ENTITY_FAMILY					2

class	AEntity
{
	public:
		static const uint	value_type;

		virtual ~AEntity();
		uint				getType() const;
		uint				getFamily() const;
		const std::string	&getUID() const;
		void				setUID( const std::string & uid );
	
	protected:
		AEntity( const int type, const std::string &uid );

		uint				_type;
		uint				_family;
		std::string			_uid;
};

const uint AEntity::value_type = ENTITY_TYPE;

#endif /* ******************************************************** AENTITY_HPP */
