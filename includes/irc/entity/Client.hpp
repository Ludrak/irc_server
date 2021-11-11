#ifndef CLIENT_HPP
# define CLIENT_HPP

class IRCServer;
# include "SockStream.hpp"
# include "AEntity.hpp"
# include "IRCServer.hpp"

# define NB_CLIENT_REGISTRATION_MAX 12

class Client : public AEntity
{
//TODO pass all getter in const std::string &
	public:

		Client(SockStream & socket);
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

		const std::string&	getVersion( void );
		void				setVersion(std::string version); //Server only

		const std::string&	getFlags( void );
		void				setFlags(std::string flags); //Server only

		void				setRegistered( bool registered );
		bool				isRegistered( void );

		uint				getType( void ) const;
		SockStream&			getStream();


		uint				joinChannel( Channel & chan ); //client only
		uint				leaveChannel( Channel & chan ); //client only
		uint				leaveAllChannels( void ); //client only

	
	private:

		Client( Client const & src );
		std::list<Channel *>				_channels;
		std::string							_username;
		std::string							_domaine;
		std::string							_servername;
		std::string							_realname;
		std::string							_version; //Server only
		std::string							_flags; //Server only
		bool								_registered;
		SockStream*							_socket;

};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */