#ifndef CLIENT_HPP
# define CLIENT_HPP

class IRCServer;
# include "SockStream.hpp"
# include "AEntity.hpp"
# include "IRCServer.hpp"
# include "NetworkEntity.hpp"
# include "ClientInfo.hpp"
# include "UnRegisteredConnection.hpp"



/*
class Client : public AEntity
{
//TODO pass all getter in const std::string &
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

		const std::string&	getVersion( void );
		void				setVersion(std::string version); //Server only

		const std::string&	getFlags( void );
		void				setFlags(std::string flags); //Server only

		void				setRegistered( bool registered );
		uint				getHopCount() const;
		void				setHopCount( uint hop );

		const std::string	&getSID() const;
		void				setSID( const std::string & sid );

		const std::string	&getServerDescription() const;
		void				setServerDescription(const std::string &desc);

		void				setRegisteredAsClient( bool registered );
		void				setRegisteredAsServer( bool registered );
		//REVIEW isRegistered makred twice
		bool				isRegistered( void );

		uint				getType( void ) const;
		SockStream&			getStream() const;


		uint				joinChannel( Channel & chan ); //client only
		uint				leaveChannel( Channel & chan ); //client only
		uint				leaveAllChannels( void ); //client only

	
		bool				isRelayed() const;
		void				setRelayed( bool relayed );

		enum C { 
			value_type_unknown = 0,
			value_type_client = 1,
			value_type_server = 2 
		};
	
	private:
		Client( Client const & src );

		std::string							_username;
		std::string							_domaine;
		std::string							_servername;
		std::string							_realname;
		std::string							_version; //Server only
		std::string							_flags; //Server only
		bool								_registered;
		SockStream*							_socket;
		int									_type;
		std::list<Channel *>				_channels;

		uint								_hopcount;
		std::string							_sid;
		std::string							_serverinfo;

		bool								_isRelayed;
};

std::ostream &			operator<<( std::ostream & o, Client const & i );
*/

# define CLIENT_ENTITY_TYPE	3

class Client : public NetworkEntity, public ClientInfo
{
	public:
		static const int	value_type;
	
	public:
		Client (const UnRegisteredConnection &client);
};

const int Client::value_type = CLIENT_ENTITY_TYPE;

#endif /* ********************************************************** CLIENT_H */