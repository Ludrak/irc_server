#ifndef AENTITY_HPP
# define AENTITY_HPP

# include <iostream>
# include <string>
# include <list>
# include <map>

class AEntity
{

	public:

		AEntity();
		AEntity( AEntity const & src );
		virtual ~AEntity();

		AEntity &		operator=( AEntity const & rhs );

		uint					getUID( void ) const;
		virtual uint			getType( void ) const = 0;
		
		std::string			getNickname( void );
		void				setNickname(std::string nick);
	
		std::string			getPassword( void );
		void				setPassword(std::string password);

	private:
		uint				_uid;
		std::string			_nickname;
		std::string			_givenPassword;

};

std::ostream &			operator<<( std::ostream & o, AEntity const & i );

#endif /* ******************************************************** AENTITY_HPP */