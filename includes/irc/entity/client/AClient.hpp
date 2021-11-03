#ifndef ACLIENT_HPP
# define ACLIENT_HPP

# include "SockStream.hpp"
# include "AEntity.hpp"

class AClient : public AEntity
{

	public:

		AClient(IProtocol * protocol, SockStream & socket);
		virtual ~AClient();

		SockStream&			getStream();
		std::string			getPassword( void );
		void				setPassword(std::string password);

	private:
		AClient &		operator=( AClient const & rhs );
		IProtocol*							_protocol;
		SockStream *						_socket;
		std::string							_givenPassword;

};

#endif /* ********************************************************* ACLIENT_H */