#ifndef CLIENT_HPP
# define CLIENT_HPP

class IRCServer;
# include "SockStream.hpp"
# include "AEntity.hpp"
# include "IRCServer.hpp"

class Client : public AEntity
{

	public:

		Client(const SockStream & socket);
		virtual ~Client();

		Client &			operator=( Client const & rhs );

		std::string			getUsername( void );
		void				setUsername(std::string user);

		std::string			getDomaineName( void );
		void				setDomaineName(std::string domaine);
		
		std::string			getServername( void );
		void				setServername(std::string servername);

		std::string			getRealname( void );
		void				setRealname(std::string realName);

		void				setRegistered( bool registered );
		bool				isRegistered( void );

		uint				getType( void ) const;
		SockStream&			getStream();

		enum C { value_type = 1 };
	
	private:

		Client( Client const & src );
		std::string							_username;
		std::string							_domaine;
		std::string							_servername;
		std::string							_realname;
		bool								_registered;
		IRCServer*							_master;
		ushort								_socket;

};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */