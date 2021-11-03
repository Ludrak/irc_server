#ifndef ACLIENT_HPP
# define ACLIENT_HPP

# include "SockStream.hpp"
# include "AEntity.hpp"

class AClient : public AEntity
{

	public:

		AClient(IProtocol * protocol, SockStream & socket);
		virtual ~AClient();

		bool				isRegistered( void );
		SockStream&			getStream();
		std::string			getPassword( void );
		std::string			getNickname( void );
		void				setRegistered( bool registered );
		void				setNickname(std::string nick);
		void				setPassword(std::string password);

	private:
		AClient &		operator=( AClient const & rhs );
		IProtocol*							_protocol;
		SockStream *						_socket;
		std::string							_givenPassword;
		std::string							_nickname;
		bool								_registered;

};

#endif /* ********************************************************* ACLIENT_H */