#ifndef AENTITY_HPP
# define AENTITY_HPP

# include <iostream>
# include <string>
# include <list>
# include <map>
# include "status.hpp"
# include "CommonInfo.hpp"

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

class	AEntity : public CommonInfo
{
	public:
		static const uint	value_type = ENTITY_TYPE;

		virtual ~AEntity();
		uint					getType() const;
		uint					getFamily() const;
		const struct timeval	&getCreationTime() const;

		virtual const std::string		getPrefix( void ) const;
	protected:
		AEntity( const int type, const std::string &uid, const struct timeval * time = NULL);

		uint				_type;
		uint				_family;
		struct timeval		_creationTime;
};

#endif /* ******************************************************** AENTITY_HPP */
