#ifndef CHANBOT_HPP
# define CHANBOT_HPP

# include <iostream>
# include <string>
# include <fstream>
# include "AClient.hpp"
# include "Parser.hpp"
# include "IRCProtocol.hpp"

class ChanBot : public AClient
{

	public:

		ChanBot(const std::string & name="tony", const std::string & host="localhost", ushort port=6667, const std::string & password="", const bool useTLS=false);
		ChanBot( ChanBot const & src );
		~ChanBot();

		ChanBot &		operator=( ChanBot const & rhs );

		bool			connect( void );

		void			_onConnect ( SockStream &server);
        void			_onRecv( SockStream &server,  Package &pkg);
	    void			_onQuit( SockStream &server);

		void			parseMessage(std::string & message, uint nbParam);
		void			parseNumericMessage(std::string & message, uint val);
		void			addNewChannel(std::string & message);
		void			validNewChannel(std::string & message);
		void			handleMessage(std::string & message);

		bool			inappropriateCheck(std::string & message);
		bool			loadDict(std::string & dictPath);
		void			printList( void );

	
	private:
		std::vector<std::string>	_dict;
		std::vector<std::string>	_channels;
		std::list<std::string>		_pendingChan;

		std::string					_name;
		std::string					_host;
		std::string					_password;
		ushort						_port;
		IRCProtocol					_protocol;

		SockStream*					_currentStream;
		std::string					_currentPrefix;		
		


};

#endif /* ********************************************************* CHANBOT_H */