#ifndef CLIENT_HPP
# define CLIENT_HPP

class IRCServer;

# include "AEntity.hpp"
# include "IRCServer.hpp"
# include "AClient.hpp"

class Client : public AClient
{

	public:

		Client(IRCServer &master, SockStream & socket);
		virtual ~Client();

		Client &			operator=( Client const & rhs );

		uint				getId( void ) const;
		void				setUsername(std::string user);
		void				setDomaineName(std::string domaine);
		void				setServername(std::string servername);
		void				setRealname(std::string realName);
		std::string			getUsername( void );
		std::string			getDomaineName( void );
		std::string			getServername( void );
		std::string			getRealname( void );

		enum C { value_type = 1 };
	
	private:

		Client( Client const & src );
		std::string							_username;
		std::string							_domaine;
		std::string							_servername;
		std::string							_realname;
		AServer*							_master;
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */