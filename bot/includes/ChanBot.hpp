#ifndef CHANBOT_HPP
# define CHANBOT_HPP

# include <iostream>
# include <string>
# include "AClient.hpp"
# include "IRCProtocol.hpp"

class ChanBot : public AClient
{

	public:

		ChanBot(const std::string & host, ushort port, std::string & password, const bool useTLS=false);
		ChanBot( ChanBot const & src );
		~ChanBot();

		ChanBot &		operator=( ChanBot const & rhs );

		bool			connect( void );


		void			_onConnect ( SockStream &server);
        void			_onRecv( SockStream &server,  Package &pkg);
	    void			_onQuit( SockStream &server);
	
	private:
		std::list<std::string>		_dict;

		std::string					_name;
		std::string					_host;
		std::string					_password;
		ushort						_port;
		IRCProtocol					_protocol;
		


};

#endif /* ********************************************************* CHANBOT_H */