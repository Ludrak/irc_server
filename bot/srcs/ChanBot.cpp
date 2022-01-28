#include "ChanBot.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ChanBot::ChanBot(const std::string & name, const std::string & host, ushort port, const std::string & password, const bool useTLS)
:	_name(name),
	_host(host),
	_password(password),
	_port(port),
	_preventingMessage("Hey you little brat!!!! You've been catched using an inappropriate language. You have been reported to the administrator, so don't do it again!"),
	_currentStream(NULL)
{
	Logger::debug("ChanBot contructor");
	this->_useTLS = useTLS;
	std::string dictPath("./conf/wordlist.txt");
	if (this->loadDict(dictPath) == false)
		Logger::error("failed to load dict file");
	// printList();
}

ChanBot::ChanBot( const ChanBot & src )
{
	this->operator=(src);
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
	if ( this != &rhs )
	{
		this->_dict = rhs._dict;
		this->_channels = rhs._channels;
		this->_pendingChan.clear();
		this->_name = rhs._name;
		this->_host = rhs._host;
		this->_password = rhs._password;
		this->_port = rhs._port;
		this->_currentStream = NULL;
		this->_currentPrefix = "";
	}
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
		Logger::debug("Prefix: " + this->_currentPrefix);
		nbParam--;
	}
	else
		this->_currentPrefix.clear();
	uint				val = 0;
	std::string			cmd(Parser::extractFirst(message));
	std::istringstream	is(cmd);

	is >> val;
	if (val != 0)
		return parseNumericMessage(message, val);

	Logger::info("Not a numeric message");
	if (cmd == "JOIN")
		this->validNewChannel(message);
	else if (cmd == "PRIVMSG")
		this->handleMessage(message);
	return ;
}

void			ChanBot::parseNumericMessage(std::string & message, uint val)
{
	
	Logger::info("numeric value: " + ntos(val));
	Logger::debug("Message: " + message);
	/* Check if message is for us */
	std::string recipient = Parser::extractFirst(message);
	Logger::debug("recipient: " + recipient);
	if (recipient != this->_name)
		return Logger::debug("Message not for us");
	else if (val == 322)
	{
		/* Get channel name */
		std::string chanName = Parser::getParam(message, 0);
		/* add a channel to the watching list */
		return joinChannel(chanName);
	}
	else{
		Logger::debug("message dropped.");
	}
	return ;
}

void			ChanBot::validNewChannel(std::string & message)
{
	/* Receiving the server validation for joining a channel */
	std::list<std::string>::iterator fd = std::find(this->_pendingChan.begin(), this->_pendingChan.end(), message);
	if (fd == this->_pendingChan.end())
	{
		Logger::info("Request to join: " + message);
		message = "JOIN " + message;
		Package* package = new Package(this->_protocol, this->_protocol.format(message), this->_currentStream);
		this->sendPackage(package, *this->_currentStream);
		return ;	
	}
	this->_pendingChan.remove(message);
	this->_channels.push_back(message);
	Logger::info("validChan: new channel <" + message + "> registered");
	return ;
}

void			ChanBot::handleInappropriateWord(const std::string & target)
{
	std::string message;
	std::string userName = this->getUsernameFromPrefix(this->_currentPrefix);
	Logger::warning("handleMessage: message from <" + userName + ">(" + this->_currentPrefix + ") contain some inappropriate content: reporting");
	
	/* Kick user from channel */
	message = "KICK " + target + " " + userName + " :the use of inappropriate language is prohibited";
	Package* package = new Package(this->_protocol, this->_protocol.format(message), this->_currentStream);
	this->sendPackage(package, *this->_currentStream);

	/* Send a private message to the abusive client */
	message = "PRIVMSG " + userName + " :" + this->_preventingMessage;
	package = new Package(this->_protocol, this->_protocol.format(message), this->_currentStream);
	this->sendPackage(package, *this->_currentStream);
}

void			ChanBot::handleMessage(std::string & message)
{
	Logger::debug("receiving msg = (" + message + ")");
	std::string target(Parser::extractFirst(message));
	if (target == this->_name)
		return Logger::warning("Receive a personal message from <" + this->_currentPrefix + ">: " + message);
	/* Check if we are on this channel */
	std::vector<std::string>::iterator fd = std::find(this->_channels.begin(), this->_channels.end(), target);
	if (fd == this->_channels.end())
	{
		Logger::info("handleMessage: Not on Channel <" + target + ">");
		return this->joinChannel(target);
	}
	else if (this->inappropriateWordCheck(message) == true)
		this->handleInappropriateWord(target);
	return ;
}

void			ChanBot::joinChannel(const std::string & chanName)
{
	Logger::info("ChanName: " + chanName);
	/* Check if not already in channel */
	std::vector<std::string>::iterator fd = std::find(this->_channels.begin(), this->_channels.end(), chanName);
	if (fd != this->_channels.end())
		return Logger::info("newChan: Already registered");
	/* Ask to join channel */
	Logger::info("Request to join: " + chanName);
	Package* package = new Package(this->_protocol, this->_protocol.format("JOIN " + chanName), this->_currentStream);
	this->sendPackage(package, *this->_currentStream);
	/* Add it to the list of requested channels */
	this->_pendingChan.push_back(chanName);
}

bool		ChanBot::inappropriateWordCheck(std::string & message)
{
	for (std::vector<std::string>::const_iterator it = this->_dict.begin(); it != this->_dict.end(); ++it)
	{
		if (message.find(*it) != std::string::npos)
			return true;
	}
	return false;
}

bool		ChanBot::loadDict(std::string & dictPath)
{
	std::ifstream ifs(dictPath);
	if (!ifs.is_open() || !Parser::isRegularFile(dictPath.c_str()))
	{
		Logger::error("Cannot open/read file: " + dictPath);
		return false;
	}
	Logger::debug("Dict file: " + dictPath + " opened.");
	for (std::string line; std::getline(ifs, line);)
	{
		this->_dict.push_back(line);
	}
	Logger::info("Dict file loaded.");
	return true;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void		ChanBot::printList( void )
{
	uint i = 0;
	for (std::vector<std::string>::const_iterator it = this->_dict.begin(); it != this->_dict.end(); ++it)
	{
		std::cout << *it << ", ";
		if (i % 12 == 0)
			std::cout << std::endl;
		++i;
	}
	std::cout << "Nb entry: " << i << std::endl;
}

std::string		ChanBot::getUsernameFromPrefix(const std::string & prefix)
{
	size_t fullMark = prefix.find('@');
	if ( fullMark == std::string::npos)
	{
		/* simple prefix */
		return prefix.substr(1);
	}
	/* full prefix */
	size_t userMark = prefix.find('!');
	if ( userMark != std::string::npos)
	{
		/* full prefix with username */
		return prefix.substr(1, userMark - 1);
	}
	/* full prefix without username */
		return prefix.substr(1, fullMark - 1);
	
}

/* ************************************************************************** */