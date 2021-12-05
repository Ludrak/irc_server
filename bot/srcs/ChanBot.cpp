#include "ChanBot.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ChanBot::ChanBot(const std::string & host, ushort port, std::string & password, const bool useTLS) : _name("tony")
{
	Logger::debug("ChanBot contructor");
	this->_host = host;
	this->_port = port;
	this->_password = password;
	this->_useTLS = useTLS;
}

ChanBot::ChanBot( const ChanBot & src )
{
	(void) src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ChanBot::~ChanBot()
{
	Logger::debug("ChanBot destructor");
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ChanBot &				ChanBot::operator=( ChanBot const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	(void) rhs;
	return *this;
}


/*
** --------------------------------- METHODS ----------------------------------
*/



bool							ChanBot::connect()
{
	Logger::info("Try connecting to network:"); 
	Logger::info("- host     : " + this->_host); 
	Logger::info("- port     : " + ntos(this->_port)); 
	Logger::info("- password : " + this->_password);

	try
	{
		if (this->connectOn(this->_host, this->_port, this->_protocol, this->_useTLS) == false)
		{
			Logger::error("Cannot connect to network");
			return false;
		}
		this->_running = true;
	}
	catch (const AClient::ConnectionException &e)
	{
		/*
			connection exception, we are root on that server
			until another connection succeeds
		*/
		Logger::warning ("Connection to server failed: aborting operations");
		return false;
	}
	return true;
}

/*
** --------------------------------- EVENTS ---------------------------------
*/

void			ChanBot::_onConnect ( SockStream &server)
{
	Logger::warning("Connection with " + this->_host + ":" + ntos(this->_port) + " succeed");
	Logger::info("Sending authentication infos");
	std::string message;
	Package *package = NULL;
	if (!this->_password.empty())
	{
		message = "PASS " + this->_password;
		package = new Package(this->_protocol, this->_protocol.format(message), &server);
		this->sendPackage(package, server);
	}
	message = "NICK " + this->_name;
	package = new Package(this->_protocol, this->_protocol.format(message), &server);
	this->sendPackage(package, server);

	message = "USER " + this->_name + " jsp jsp :Tony le Bot";
	package = new Package(this->_protocol, this->_protocol.format(message), &server);
	this->sendPackage(package, server);

	message = "OPER becomeOper becomeOper";
	package = new Package(this->_protocol, this->_protocol.format(message), &server);
	this->sendPackage(package, server);

	message = "LIST";
	package = new Package(this->_protocol, this->_protocol.format(message), &server);
	this->sendPackage(package, server);
}

void			ChanBot::_onRecv( SockStream &server,  Package &pkg)
{
	std::string payload = pkg.getData();
	Logger::debug("Received: " + payload + " from " + ntos(server.getSocket()));
}

void			ChanBot::_onQuit( SockStream &server)
{
	Logger::warning("Quiting connection with: " + ntos(server.getSocket()));
	this->_running = false;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */