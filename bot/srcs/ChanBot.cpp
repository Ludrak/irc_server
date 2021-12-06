#include "ChanBot.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ChanBot::ChanBot(const std::string & host, ushort port, std::string & password, const bool useTLS)
: _name("tony"), _host(host), _password(password), _port(port), _currentStream(NULL)
{
	Logger::debug("ChanBot contructor");
	this->_useTLS = useTLS;
	this->_dict.push_back("dick");
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


void			ChanBot::_onQuit( SockStream &server)
{
	Logger::warning("Quiting connection with: " + ntos(server.getSocket()));
	this->_running = false;
}

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
	if (payload.empty())
		return ;
	Logger::debug("Received: " + payload + " from " + ntos(server.getSocket()));
	uint nbParam = Parser::nbParam(payload);
	if (nbParam > 1)
	{
		this->_currentStream = &server;
		parseMessage(payload, nbParam);
	}
	return ;
}

void			ChanBot::parseMessage(std::string & message, uint nbParam)
{
	std::string prefix;
	if (message[0] == ':')
	{
		/* There is a prefix */
		this->_currentPrefix = Parser::extractFirst(message);
		Logger::debug("Prefix: " + prefix);
		Logger::debug("Message: " + message);
		nbParam--;
	}
	else
		this->_currentPrefix.clear();
	uint val = 0;
	std::string cmd(Parser::extractFirst(message));
	std::istringstream is(cmd);
	is >> val;
	if (val != 0)
	{
		return parseNumericMessage(message, val);
	}
	Logger::info("Not a numeric message");
	if (cmd == "JOIN")
	{
		this->validNewChannel(message);
	}else if (cmd == "PRIVMSG")
	{
		this->handleMessage(message);
	}
	return ;
}

void			ChanBot::parseNumericMessage(std::string & message, uint val)
{
	
	Logger::warning("Handling value: " + ntos(val));
	Logger::debug("Message: " + message);
	/* Check if message is for us */
	std::string recipient = Parser::extractFirst(message);
	Logger::debug("recipient: " + recipient);
	if (recipient != this->_name)
		return Logger::debug("Message not for us");
	if (val == 322)
	{
		/* add a channel to the watching list */
		return addNewChannel(message);
	}

	return ;
}

void			ChanBot::addNewChannel(std::string & message)
{
	/* Get channel name */
	std::string chanName = Parser::getParam(message, 0);
	Logger::info("ChanName: " + chanName);
	/* Check if not already in channel */
	std::vector<std::string>::iterator fd = std::find(this->_channels.begin(), this->_channels.end(), chanName);
	if (fd != this->_channels.end())
		return Logger::info("newChan: Already registered");
	/* Ask to join channel */
	Logger::info("Request to join: " + chanName);
	message = "JOIN " + chanName;
	Package* package = new Package(this->_protocol, this->_protocol.format(message), this->_currentStream);
	this->sendPackage(package, *this->_currentStream);
	/* Add it to the list of requested channels */
	this->_pendingChan.push_back(chanName);
}

void			ChanBot::validNewChannel(std::string & message)
{
	/* Receiving the server validation for joining a channel */
	std::list<std::string>::iterator fd = std::find(this->_pendingChan.begin(), this->_pendingChan.end(), message);
	if (fd == this->_pendingChan.end())
		return Logger::error("validChan: Not pending");	
	this->_pendingChan.remove(message);
	this->_channels.push_back(message);
	Logger::info("validChan: new channel <" + message + "> registered");
	return ;
}

void			ChanBot::handleMessage(std::string & message)
{
	Logger::debug("msg = (" + message + ")");
	std::string target(Parser::extractFirst(message));
	if (target == this->_name)
		return Logger::warning("Receive a personal message from <" + this->_currentPrefix + ">:" + message);
	else if (this->inappropriateCheck(message) == true)
	{
		/* Message contain some inappropriate content */
		Logger::warning("handleMessage: message from <" + this->_currentPrefix + "> contain some inappropriate content: reporting");
		/* Check if we are on this channel */
		std::vector<std::string>::iterator fd = std::find(this->_channels.begin(), this->_channels.end(), target);
		if (fd != this->_channels.end())
			return Logger::info("handleMessage: Not on Channel <" + target + ">");
		
	}
	return ;
}

bool		ChanBot::inappropriateCheck(std::string & message)
{
	for (std::vector<std::string>::const_iterator it = this->_dict.begin(); it != this->_dict.end(); ++it)
	{
		if (message.find(*it) != std::string::npos)
			return true;
	}
	return false;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */