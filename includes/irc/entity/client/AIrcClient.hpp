#ifndef AIrcClient_HPP
# define AIrcClient_HPP

# include "SockStream.hpp"
# include "AEntity.hpp"

class AIrcClient : public AEntity
{

	public:

		AIrcClient(SockStream & socket);
		virtual ~AIrcClient();

		bool				isRegistered( void );
		SockStream&			getStream();
		std::string			getPassword( void );
		std::string			getNickname( void );
		void				setRegistered( bool registered );
		void				setNickname(std::string nick);
		void				setPassword(std::string password);

	private:
		AIrcClient &						operator=( AIrcClient const & rhs );
		SockStream *						_socket;
		std::string							_givenPassword;
		std::string							_nickname;
		bool								_registered;

};

#endif /* ********************************************************* AIrcClient_H */