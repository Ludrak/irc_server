#include "IRCServer.hpp"
#include <exception>
#include <iomanip>

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

int		printUsage(void)
{
	Logger::info("ft_irc by Nlecaill and Lrobino.");
	Logger::info("Usage:");
	Logger::info("--servname <name>     : specifies a name for the server, usefull for server-server connections");
	Logger::info("--token <token>       : specifies a token which is the SUID for the server, must be unique through all network  !");
	Logger::info("--info <info>         : specifies some additional infos for the server");
	Logger::info("--max-connections <n> : specifies a number of maxium connection for that server");
	Logger::info("--crlf                : use plain text crlf for data transmission (! default !) ");
	// Logger::info("--ssl                 : uses ssl for data transmission");
	Logger::info("--tls-port <port>     : specifies ssl port for incomming connections and data encryption");
	Logger::info("--ssl-cert <cert>     : specifies ssl certificate for incomming connections and data encryption");
	Logger::info("--ssl-key <key>       : specifies ssl key for incomming connections and data encryption");
	exit(SUCCESS);
	return SUCCESS;
}

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

bool	isValidServerToken(const std::string &tok)
{
	if (tok.size() > 3 || tok.find_first_not_of("0123456789") != std::string::npos)
		return (false);
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

bool	isValidServerHost(const std::string &str)
{
	// TODO check host validity
	(void)str;
	return (true);
}

bool    isValidSSLCert(const std::string &str)
{
	(void)str;
	return true;
}

bool	isNumber(const std::string &str)
{
	return (!str.empty() && str.find_first_not_of("0123456789") == std::string::npos);
}

bool	isValidPort(const std::string &str)
{
	return (isNumber(str));
}

# define ARG_NOTFOUND	0
# define ARG_ERROR		1
# define ARG_OK			2
uint	getArg(const char *const identifier,
				const int n,
				const int ac,
				const char *const *const argv,
				bool (*validityChecker)(const std::string &)=NULL,
				bool asArgument=true)
{
	if (n < ac && std::strcmp(argv[n], identifier) == 0)
	{
		if (asArgument == false)
			return (ARG_OK);
		if (n + 1 < ac)
		{
			if (!validityChecker(argv[n + 1]))
			{
				Logger::error("arument " + std::string(identifier) + " is invalid: " + argv[n + 1]);
				return (ARG_ERROR);
			}
			return (ARG_OK);
		}
		Logger::error("arument " + std::string(identifier) + " has no value after it");
		return (ARG_ERROR);
	}
	return (ARG_NOTFOUND);
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

void	printUsage(std::string exec_name)
	{
	std::cout
	<< "\033[1;33m**\033[0m \033[1;33m" << exec_name << " help page" << std::endl << "\033[0m\033[1;33m**\033[0m" << std::endl
	<< "\033[1;37m**\033[0m \033[1;37mUsage:\033[0m" << std::endl
	<< "\033[1;33m**\033[0m -> " << exec_name << " \033[1;37m[\033[0mnetwork_host\033[1;37m:\033[0mnetwork_port\033[1;37m:\033[0mnetwork_password\033[1;37m]\033[0m <server_port> [server_password] [options]" << std::endl << "\033[1;33m**\033[0m" << std::endl
	<< "\033[1;33m**\033[0m network_host            Sets the hostname of the irc network you want to connect to." << std::endl
	<< "\033[1;33m**\033[0m network_port            Sets the port of the irc network you want to connect to." << std::endl
	<< "\033[1;33m**\033[0m network_password        Sets the password of the irc network you want to connect to." << std::endl
	<< "\033[1;33m**\033[0m" << std::endl
	<< "\033[1;33m**\033[0m server_port             Sets the port on which this server will accept new connections." << std::endl
	<< "\033[1;33m**\033[0m server_password         Sets the password that will be asked to clients and server" << std::endl
	<< "\033[1;33m**\033[0m                         trying to connect on this server." << std::endl
	<< "\033[1;33m**\033[0m " << std::endl
	<< "\033[1;37m**\033[0m \033[1;37mOptions:\033[0m" << std::endl << "\033[1;33m**\033[0m" << std::endl
	<< "\033[1;33m**\033[0m --host <hostname>       Sets the host of server." << std::endl << "\033[1;33m**\033[0m"
	<< std::endl
	<< "\033[1;33m**\033[0m --servname <name>       Sets the name of the current server." << std::endl << "\033[1;33m**\033[0m"
	<< std::endl
	<< "\033[1;33m**\033[0m --token <token>         Sets the token of the server (3 digits max number)." << std::endl << "\033[1;33m**\033[0m"
	<< std::endl
	<< "\033[1;33m**\033[0m --info <info>           Sets the info of the current server." << std::endl
	<< "\033[1;33m**\033[0m                         This info is sent to other servers in the " << std::endl
	<< "\033[1;33m**\033[0m                         network via SERVER command." << std::endl << "\033[1;33m**\033[0m"
	<< std::endl
	<< "\033[1;33m**\033[0m --max-connextions <nb>  Sets the maximum number of connections that the server" << std::endl
	<< "\033[1;33m**\033[0m                         can handle, which is set to 20 by defaul.t" << std::endl << "\033[1;33m**\033[0m"
	<< std::endl
	<< "\033[1;37m**\033[0m \033[1;37mTLS Options:\033[0m" << std::endl << "\033[1;33m**\033[0m" << std::endl
	<< "\033[1;33m**\033[0m --tls-port              Sets the port on wich the server will listen on tls." << std::endl
	<< "\033[1;33m**\033[0m                         This is set by default to 6697." << std::endl << "\033[1;33m**\033[0m"
	<< std::endl
	<< "\033[1;33m**\033[0m --ssl-cert              Sets the SSL certificate to use for encrypted communications" << std::endl << "\033[1;33m**\033[0m"
	<< std::endl
	<< "\033[1;33m**\033[0m --ssl-key               Sets the SSL private key to use for authentification to the server" << std::endl;
}




int		main(int ac, char **av)
{
	Logger::init(CORE);
	bool		has_network_connection = false;
	std::string	network_host = "";
	ushort		network_port = 0;
	std::string	network_pass = "";

	std::string	server_host = "127.0.0.1";
	ushort		server_port = 0;
	std::string	server_pass = "";
	std::string	server_name = "irc";
	int			server_token = 0;
	std::string server_info = "Default server informations;";
	uint		server_max_connections = 20;
	std::string ssl_cert;
	std::string ssl_key;
	ushort		tls_port;
//	bool		use_crlf = true; // unimpemented ssl

	// get forward server arg
	int	argn = 1;
	if (ac <= 1)
		return printUsage();
	else if (ac > 1) {
		has_network_connection = getForward(av[1], &network_host, &network_port, &network_pass);
		if (has_network_connection)
			argn = 2;
	}
	int arg_code;;
	// get last arguments 
	while (argn < ac)
	{
		arg_code = ARG_NOTFOUND;

		/* display help */
		arg_code = getArg("--help", argn, ac, av, NULL, false);
		if (arg_code == ARG_OK)
		{
			printUsage();	
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);

		/* server host */
		arg_code = getArg("--host", argn, ac, av, isValidServerHost);
		if (arg_code == ARG_OK)
		{
			server_host = std::string(av[argn + 1]);
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);

		/* server name */
		arg_code = getArg("--servname", argn, ac, av, isValidServerName);
		if (arg_code == ARG_OK)
		{
			server_name = std::string(av[argn + 1]);
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);
		
		/* server token */
		arg_code = getArg("--token", argn, ac, av, isValidServerToken);
		if (arg_code == ARG_OK)
		{
			std::istringstream is(av[argn + 1]);
			is >> server_token;
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);

		/* server info */
		arg_code = getArg("--info", argn, ac, av, isValidServerInfo);
		if (arg_code == ARG_OK)
		{
			server_info = std::string(av[argn + 1]);
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

			is >> server_max_connections;
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);

		/* SSL pem location */
		arg_code = getArg("--ssl-cert", argn, ac, av, isValidSSLCert);
		if (arg_code == ARG_OK)
		{
			ssl_cert = std::string(av[argn + 1]);
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);

		arg_code = getArg("--ssl-key", argn, ac, av, isValidSSLCert);
		if (arg_code == ARG_OK)
		{
			ssl_key = std::string(av[argn + 1]);
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);
		
		arg_code = getArg("--tls-port", argn, ac, av, isValidPort);
		if (arg_code == ARG_OK)
		{
			std::istringstream is(av[argn + 1]);
			is >> tls_port;
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);

		/* help */
		if (std::strcmp(av[argn], "--help") == 0)
		{
			printUsage(av[0]);
			return (EXIT_SUCCESS);
		}

		/* unknown argument */
		if (std::strncmp(av[argn], "--", 2) == 0)
		{
			Logger::error("unknown argument: " + ntos(av[argn]));
			printUsage();
			return (EXIT_FAILURE);
		}
		/* port */
		else if (isNumber(av[argn]) && server_port == 0)
		{
			std::istringstream is(av[argn++]);
			is >> server_port;
		}
		/* password */
		else if (server_pass.empty())
			server_pass = std::string(av[argn++]);
		else 
		{
			Logger::error("too many unspecified agruments");
			return (EXIT_FAILURE);
		}
	}

	if (server_port == 0)
	{
		Logger::error("No port specified");
		return (EXIT_FAILURE);
	}


	std::stringstream ss;
	ss << std::setfill('0') << std::setw(3) << server_token;
	
	Logger::info("*************************************************");
	Logger::info("* IRC Server (" + server_name + "@" +  server_host + ")");
	Logger::info("*");
	if (has_network_connection)
	{
		Logger::info("* Network:");
		Logger::info("* - host:            " + network_host);
		Logger::info("* - port:            " + ntos(network_port));
		Logger::info("* - pass:            " + network_pass);
		Logger::info("*");
	}
	Logger::info("* Server:");
	Logger::info("* - host:            " + server_host);
	Logger::info("* - port:            " + ntos(server_port));
	Logger::info("* - password:        " + server_pass);
	Logger::info("*");

	Logger::info("* Server infos:");
	Logger::info("* - name:            " + server_name);
	Logger::info("* - token:           " + ss.str());
	Logger::info("* - max connections: " + ntos(server_max_connections));
	Logger::info("* - info:            " + server_info);
	Logger::info("*************************************************");

	IRCServer server(server_port, server_pass, server_host, ssl_cert, ssl_key, tls_port);

	server.setName(server_name);
	server.setUID(ss.str());
	server.setInfo(server_info);
	server.setDebugLevel(false);
	server.setMaxConnection(server_max_connections);
	if (has_network_connection)
		server.connectToNetwork(network_host, network_port, network_pass);
	else
		Logger::info("No forward connection set: running server as root");
	server.run();
	return (EXIT_SUCCESS);
}