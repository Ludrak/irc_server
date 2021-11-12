#ifndef CLIENT_HPP
# define CLIENT_HPP

class IRCServer;
# include "SockStream.hpp"
# include "AEntity.hpp"
# include "IRCServer.hpp"

class Client : public AEntity
{

	public:

		Client(SockStream & socket);
		virtual ~Client();

		Client &			operator=( Client const & rhs );

		const std::string	&getUsername( void ) const;
		void				setUsername(std::string user);

		const std::string	&getDomaineName( void ) const;
		void				setDomaineName(std::string domaine);
		
		const std::string	&getServername( void ) const;
		void				setServername(std::string servername);

		const std::string	&getRealname( void ) const;
		void				setRealname(std::string realName);

		uint				getHopCount() const;
		void				setHopCount( uint hop );

		const std::string	&getSID() const;
		void				setSID( const std::string & sid );

		const std::string	&getServerDescription() const;
		void				setServerDescription(const std::string &desc);

		void				setRegisteredAsClient( bool registered );
		void				setRegisteredAsServer( bool registered );
		bool				isRegistered( void );

		uint				getType( void ) const;
		SockStream&			getStream();

		bool				isRelayed() const;
		void				setRelayed( bool relayed );

		enum C { 
			value_type_unknown = 0,
			value_type_client = 1,
			value_type_server = 2 
		};
	
	private:

		std::string							_username;
		std::string							_domaine;
		std::string							_servername;
		std::string							_realname;
		bool								_registered;
		SockStream*							_socket;
		int									_type;

		/* network */
		uint								_hopcount;
		std::string							_sid;
		std::string							_serverinfo;

		bool								_isRelayed;
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */