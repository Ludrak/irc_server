#include "IRCServer.hpp"
#include <exception>
#include <iomanip>

struct ircInit 
{
	bool		has_network_connection;
	ushort		network_port;
	std::string	network_host;;
	std::string	network_pass;;

	ushort		server_token;
	ushort		server_max_connections;;
	ushort		server_port;
	std::string	server_host;
	std::string	server_pass;
	std::string	server_name;
	std::string server_info;

	ushort		tls_port;
	std::string ssl_cert;
	std::string ssl_key	;
	std::string logFileName;
};

/*
**	Subject specified:
**	ft_irc [host:port_network:password_network] <port> <password>
**
**	Our:
**  ft_irc [host:port_network:password_network] <port> <password>
**	--servname <name>   	: specifies a name for the server, usefull for server-server connections 
**	--token <token>			: specifies a token which is the SUID for the server, must be unique through all network  !
**	--info <info>			: specifies some additional infos for the server
**	--max-connections <n> 	: specifies a number of maxium connection for that server
**	--crlf					: use plain text crlf for data transmission (! default !) 
//REVIEW: deprecated --ssl?
**	--ssl					: uses ssl for data transmission
**	--ssl-cert <cert>		: specifies ssl certificate for incomming connections and data encryption
**	--ssl-key <key>			: specifies ssl key for incomming connections and data encryption
*/


/* param checker */
//REVIEW replace by Parser::validServerName (and implement it)
bool	isValidServerName(const std::string &name)
{
	(void)name;
	return (true);
}

bool	isValidServerInfo(const std::string &info)
{
	(void)info;
	return (true);
}

bool	isValidServerMaxConnections(const std::string &param)
{
	std::istringstream is(param);
	u_long max = 0;
	is >> max;
	if (param.find_first_not_of("0123456789") != std::string::npos || max > UINT16_MAX)
		return (EXIT_FAILURE);
	return (true);
}

bool    isValidSSLCert(const std::string &str)
{
	(void)str;
	return true;
}

# define ARG_NOTFOUND	0
# define ARG_ERROR		1
# define ARG_OK			2
# define ARG_OPTIONAL	3
uint	getArg(const char *const identifier,
				const int n,
				const int ac,
				const char *const *const argv,
				bool (*validityChecker)(const std::string &)=NULL,
				bool asArgument=true,
				bool isOptional=false)
{
	if (!(n < ac && std::strcmp(argv[n], identifier) == 0))
		return (ARG_NOTFOUND);
	if (asArgument == false)
		return (ARG_OK);
	else if (n + 1 < ac)
	{
		if (!validityChecker)
		{
			if (isOptional && strncmp(argv[n + 1], "--", 2) == 0)
				return (ARG_OPTIONAL);
			else
				return (ARG_OK);
		}
		else if (!validityChecker(argv[n + 1]))
		{
			Logger::error("arument " + std::string(identifier) + " is invalid: " + argv[n + 1]);
			return (ARG_ERROR);
		}
		return (ARG_OK);
	}
	else if (isOptional)
		return (ARG_OPTIONAL);
	Logger::error("arument " + std::string(identifier) + " has no value after it");
	return (ARG_ERROR);
}




bool	getForward(std::string arg, std::string *const host, ushort *const port, std::string *const pass)
{
	std::string tmp;
	if (!host || !port || !pass)
		return false;
	// getting host
	*host = arg.substr(0, arg.find(":"));
	tmp = arg.substr(arg.find(":") + 1, arg.size() - (arg.find(":") + 1));
	if (arg == tmp)
		return (false);
	arg = tmp;

	// getting port
	*port = 0;
	std::istringstream is(arg.substr(0, arg.find(":")));
	is >> *port;
	if (*port == 0)
		return false;
	tmp = arg.substr(arg.find(":") + 1, arg.size() - (arg.find(":") + 1));
	if (arg == tmp)
		return (true);
	arg = tmp;

	// getting password
	*pass = arg;
	return (true);
}

int		printUsage(const std::string &exec_name)
{
	std::cout
	<< "\033[1;33m**\033[0m \033[1;33m" << exec_name << " by Nlecaill and Lrobino, help page\n" << 
	"\033[0m\033[1;33m**\033[0m\n" << 
	"\033[1;37m**\033[0m \033[1;37mUsage:\033[0m\n" << 
	"\033[1;33m**\033[0m -> " << exec_name << " \033[1;37m[\033[0mnetwork_host\033[1;37m:\033[0mnetwork_port\033[1;37m:\033[0mnetwork_password\033[1;37m]\033[0m <server_port> [server_password] [options]\n" << 
	"\033[1;33m**\033[0m\n" << 
	"\033[1;33m**\033[0m network_host                      Sets the hostname of the irc network you want to connect to.\n" << 
	"\033[1;33m**\033[0m network_port                      Sets the port of the irc network you want to connect to.\n" << 
	"\033[1;33m**\033[0m network_password                  Sets the password of the irc network you want to connect to.\n" << 
	"\033[1;33m**\033[0m\n" << 
	"\033[1;33m**\033[0m server_port                       Sets the port on which this server will accept new connections.\n" << 
	"\033[1;33m**\033[0m server_password                   Sets the password that will be asked to clients and server\n" << 
	"\033[1;33m**\033[0m                                   trying to connect on this server.\n" << 
	"\033[1;33m**\033[0m \n" << 
	"\033[1;37m**\033[0m \033[1;37mOptions:\033[0m\n" << 
	"\033[1;33m**\033[0m\n" << 
	"\033[1;33m**\033[0m --host <hostname>                 Set the server hostname.\n" << 
	"\033[1;33m**\033[0m\n" << 
	"\033[1;33m**\033[0m --servname <name>                 Sets the name of the current server.\n" << 
	"\033[1;33m**\033[0m\n" << 
	"\033[1;33m**\033[0m --token <token>                   Sets the token of the server (3 digits max number).\n" << 
	"\033[1;33m**\033[0m\n" << 
	"\033[1;33m**\033[0m --info <info>                     Sets the info of the current server.\n" << 
	"\033[1;33m**\033[0m                                   This info is sent to other servers in the \n" << 
	"\033[1;33m**\033[0m                                   network via SERVER command.\n" << 
	"\033[1;33m**\033[0m\n" << 
	"\033[1;33m**\033[0m --logfile 	                       Record all logs into a file in current working directory.\n" << 
	"\033[1;33m**\033[0m\n" << 
	"\033[1;33m**\033[0m --max-connections <nb>            Sets the maximum number of connections that the server\n" << 
	"\033[1;33m**\033[0m                                   can handle. Default: 20\n" << 
	"\033[1;33m**\033[0m\n" << 
	"\033[1;37m**\033[0m \033[1;37mTLS Options:\033[0m\n" << 
	"\033[1;33m**\033[0m\n" << 
	"\033[1;33m**\033[0m --tls-port                        Sets the port on wich the server will listen on tls.\n" << 
	"\033[1;33m**\033[0m                                   This is set by default to 6697.\n" << 
	"\033[1;33m**\033[0m\n" << 
	"\033[1;33m**\033[0m --ssl-cert                        Sets the SSL certificate to use for encrypted communications\n" << 
	"\033[1;33m**\033[0m\n" << 
	"\033[1;33m**\033[0m --ssl-key                         Sets the SSL private key to use for authentification to the server" << std::endl;
	return 0;
}


void	displayHeadMessage(const struct ircInit & init, const std::stringstream & formatedToken)
{
	Logger::info("*************************************************");
	Logger::info("* IRC Server (" + init.server_name + "@" +  init.server_host + ")");
	Logger::info("*");
	if (init.has_network_connection)
	{
		Logger::info("* Network:");
		Logger::info("* - host:            " + init.network_host);
		Logger::info("* - port:            " + ntos(init.network_port));
		Logger::info("* - pass:            " + init.network_pass);
		Logger::info("*");
	}
	Logger::info("* Server:");
	Logger::info("* - host:            " + init.server_host);
	Logger::info("* - port:            " + ntos(init.server_port));
	if (init.tls_port != 0)
		Logger::info("* - tls port:        " + ntos(init.tls_port));
	Logger::info("* - password:        " + init.server_pass);
	Logger::info("*");

	Logger::info("* Server infos:");
	Logger::info("* - name:            " + init.server_name);
	Logger::info("* - token:           " + formatedToken.str());
	Logger::info("* - max connections: " + ntos(init.server_max_connections));
	Logger::info("* - info:            " + init.server_info);
	Logger::info("*************************************************");

}

int		main(int ac, char **av)
{
;
//	bool		use_crlf = true; // unimpemented ssl
	struct ircInit init;
	init.has_network_connection = false;
	init.network_host	= "";
	init.network_port	= 0;
	init.network_pass	= "";
	init.server_host	= "127.0.0.1";
	init.server_name	= "irc";
	init.server_pass	= "";
	init.server_port	= 0;
	init.server_token	= 0;
	init.server_info	= "Default server informations;";
	init.server_max_connections = 20;
	init.ssl_cert		= "";
	init.ssl_key		= "";
	init.tls_port		= 0;
	init.logFileName	= "";

	Logger::init(INFO);

	// get forward server arg
	int	argn = 1;
	if (ac <= 1)
		return printUsage(av[0]);
	else if (ac > 1) {
		init.has_network_connection = getForward(av[1], &init.network_host, &init.network_port, &init.network_pass);
		if (init.has_network_connection)
			argn = 2;
	}
	int arg_code;
	// get last arguments 
	while (argn < ac)
	{
		arg_code = ARG_NOTFOUND;

		/* display help */
		arg_code = getArg("--help", argn, ac, av, NULL, false);
		if (arg_code == ARG_OK)
		{
			printUsage(av[0]);
			return (EXIT_SUCCESS);	
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);

		/* server host */
		arg_code = getArg("--host", argn, ac, av, Parser::validHostname);
		if (arg_code == ARG_OK)
		{
			init.server_host = std::string(av[argn + 1]);
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);

		/* server name */
		arg_code = getArg("--servname", argn, ac, av, isValidServerName);
		if (arg_code == ARG_OK)
		{
			init.server_name = std::string(av[argn + 1]);
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);
		
		/* server token */
		arg_code = getArg("--token", argn, ac, av, Parser::validServerToken);
		if (arg_code == ARG_OK)
		{
			std::istringstream is(av[argn + 1]);
			is >> init.server_token;
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);

		/* server info */
		arg_code = getArg("--info", argn, ac, av, isValidServerInfo);
		if (arg_code == ARG_OK)
		{
			init.server_info = std::string(av[argn + 1]);
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);

		/* max connections */
		//TODO exception not protected here and in validator
		arg_code = getArg("--max-connections", argn, ac, av, isValidServerMaxConnections);
		if (arg_code == ARG_OK)
		{
			std::istringstream is(av[argn + 1]);

			is >> init.server_max_connections;
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);

		/* SSL pem location */
		arg_code = getArg("--ssl-cert", argn, ac, av, isValidSSLCert);
		if (arg_code == ARG_OK)
		{
			init.ssl_cert = std::string(av[argn + 1]);
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);

		arg_code = getArg("--ssl-key", argn, ac, av, isValidSSLCert);
		if (arg_code == ARG_OK)
		{
			init.ssl_key = std::string(av[argn + 1]);
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);
		
		arg_code = getArg("--tls-port", argn, ac, av, Parser::validPortNumber);
		if (arg_code == ARG_OK)
		{
			std::istringstream is(av[argn + 1]);
			is >> init.tls_port;
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);

		arg_code = getArg("--logfile", argn, ac, av, NULL, true, true);
		// if (arg_code == ARG_OK)
		// {
		// 	std::istringstream is(av[argn + 1]);
		// 	is >> init.logFileName;
		// 	if (Parser::isDirectory(init.logFileName.c_str()))
		// 	{
		// 		if (init.logFileName.at(init.logFileName.size() - 1) != '/')
		// 			init.logFileName += "/";
		// 		init.logFileName += Logger::timeToString(Logger::getInitialTimestamp());
		// 	}
		// 	init.logFileName += ".irclog";
		// 	argn += 2;
		// 	continue;
		// }
		if (arg_code == ARG_OPTIONAL)
		{
			init.logFileName = Logger::timeToString(Logger::getInitialTimestamp()) + ".irclog";
			argn += 1;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);

		/* unknown argument */
		if (std::strncmp(av[argn], "--", 2) == 0)
		{
			Logger::error("unknown argument: " + ntos(av[argn]));
			return (printUsage(av[0]));
		}
		/* port */
		else if (Parser::isNumber(av[argn]) && init.server_port == 0)
		{
			std::istringstream is(av[argn++]);
			is >> init.server_port;
		}
		/* password */
		else if (init.server_pass.empty())
			init.server_pass = std::string(av[argn++]);
		else 
		{
			Logger::error("too many unspecified arguments");
			return (EXIT_FAILURE);
		}
	}

	if (init.server_port == 0)
	{
		Logger::error("No port specified");
		return (EXIT_FAILURE);
	}

	try {
		Logger::initLogfile(init.logFileName);
		std::stringstream formatedToken;
		formatedToken << std::setfill('0') << std::setw(3) << init.server_token;
		displayHeadMessage(init, formatedToken);

		IRCServer server(init.server_port, init.server_pass, init.server_host, init.ssl_cert, init.ssl_key, init.tls_port);
		server.setName(init.server_name);
		server.setUID(formatedToken.str());
		server.setInfo(init.server_info);
		server.setDebugLevel(false);
		server.setMaxConnection(init.server_max_connections);
		if (init.has_network_connection)
			server.connectToNetwork(init.network_host, init.network_port, init.network_pass, (init.tls_port != 0));
		else
			Logger::info("No forward connection set: running server as root");
		server.run();
	}
	catch (const std::exception & e)
	{
		Logger::critical(e.what());
	}
	return (EXIT_SUCCESS);
}