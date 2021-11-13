#ifndef AENTITY_HPP
# define AENTITY_HPP

# include <iostream>
# include <string>
# include <list>
# include <map>
# include "StatusCode.hpp"

# define M_REGISTRATION_LIMITED		1 << 0

/*
class AEntity
{
	public:

		AEntity(uint registrationsMax);
		AEntity( AEntity const & src );
		virtual ~AEntity();

		AEntity &		operator=( AEntity const & rhs );

		uint				getUID( void ) const;
		virtual uint		getType( void ) const = 0;
		
		std::string			getNickname( void );
		void				setNickname(std::string nick);
	
		std::string			getPassword( void );
		void				setPassword(std::string password);
		
		bool				incRegistration( void );
		bool				decRegistration( void );

		uint				getNbRegistrations( void ) const;
		uint				getNbRegistrationsMax( void ) const;

		bool				isFull( void ) const;

		void				toogleMode(uint modeMask);
		void				enableMode(uint modeMask);
		void				disableMode(uint modeMask);
		bool				isEnable(uint modeMask);

		enum C { 
			value_type_channel = 0,
			value_type_unknow = 1,
			value_type_client = 2,
			value_type_server = 3 
		};

	protected:
		uint				_nbRegistrations;
		uint				_nbRegistrationsMax;

	private:
		uint				_mode;
		uint				_uid;
		std::string			_nickname;
		std::string			_givenPassword;

};

std::ostream &			operator<<( std::ostream & o, AEntity const & i );
*/
# define ENTITY_TYPE 0

class	AEntity
{
	public:
		static const int	value_type;

		virtual ~AEntity();
		int					getType() const;
		const std::string	&getUID() const;
	
	protected:
		AEntity( const int type, const std::string &uid );

		std::string		_uid;

	private:
		int				_type;
};

const int AEntity::value_type = ENTITY_TYPE;

#endif /* ******************************************************** AENTITY_HPP */