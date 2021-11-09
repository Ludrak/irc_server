#ifndef AENTITY_HPP
# define AENTITY_HPP

# include <iostream>
# include <string>
# include <list>
# include <map>
# include "StatusCode.hpp"

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
		uint				_uid;
		std::string			_nickname;
		std::string			_givenPassword;

};

std::ostream &			operator<<( std::ostream & o, AEntity const & i );

#endif /* ******************************************************** AENTITY_HPP */