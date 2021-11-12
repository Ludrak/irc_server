#include "IRCServer.hpp"
#include <exception>

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
**	--ssl					: uses ssl for data transmission
**	--ssl-cert <cert>		: specifies ssl certificate for incomming connections and data encryption
**	--ssl-key <key>			: specifies ssl key for incomming connections and data encryption
*/

/* param checker */
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

	if (param.find_first_not_of("0123456789") != std::string::npos || static_cast<u_long>(std::stoi(param)) > UINT16_MAX)
		return (EXIT_FAILURE);
	return (true);
}

bool	isValidServerHost(const std::string &str)
{
	// TODO check host validity
	(void)str;
	return (true);
}

bool	isNumber(const std::string &str)
{
	return (!str.empty() && str.find_first_not_of("0123456789") == std::string::npos);
}

# define ARG_NOTFOUND	0
# define ARG_ERROR		1
# define ARG_OK			2
uint	getArg(const char *const identifier, const int n, const int ac, const char *const *const argv, bool (*validityChecker)(const std::string &))
{
	if (n < ac && std::strcmp(argv[n], identifier) == 0)
	{
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

	// getting host
	*host = arg.substr(0, arg.find(":"));
	tmp = arg.substr(arg.find(":") + 1, arg.size() - (arg.find(":") + 1));
	if (arg == tmp)
		return (false);
	arg = tmp;

	// getting port
	try 
	{ *port = std::stoi(arg.substr(0, arg.find(":"))); }
	catch(std::invalid_argument e)
	{ return (false); }
	tmp = arg.substr(arg.find(":") + 1, arg.size() - (arg.find(":") + 1));
	if (arg == tmp)
		return (true);
	arg = tmp;

	// getting password
	*pass = arg;
	return (true);
}




int		main(int ac, char **av)
{
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
//	bool		use_crlf = true; // unimpemented ssl

	// get forward server arg
	int	argn = 1;
	if (ac > 1) {
		has_network_connection = getForward(av[1], &network_host, &network_port, &network_pass);
		if (has_network_connection)
			argn = 2;
	}
	// get last arguments 
	while (argn < ac)
	{
		/* server host */
		int arg_code = ARG_NOTFOUND;
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
			server_token = std::stoi(av[argn + 1]);
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
		arg_code = getArg("--max-connections", argn, ac, av, isValidServerMaxConnections);
		if (arg_code == ARG_OK)
		{
			server_max_connections = std::stoi(av[argn + 1]);
			argn += 2;
			continue;
		}
		else if (arg_code == ARG_ERROR)
			return (EXIT_FAILURE);

		/* port */
		if (isNumber(av[argn]) && server_port == 0)
			server_port = std::stoi(av[argn++]);
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
		Logger::error("No port specified");
	
	std::cout << "*************************************************" << std::endl;
	std::cout << "* IRC Server (" << server_name << "@" << server_host << ")" << std::endl;
	std::cout << "*" << std::endl;
	if (has_network_connection)
	{
		std::cout << "* Network:" << std::endl;
		std::cout << "* - host:            " << network_host << std::endl;
		std::cout << "* - port:            " << network_port << std::endl;
		std::cout << "* - pass:            " << network_pass << std::endl;
		std::cout << "*" << std::endl;
	}
	std::cout << "* Server:" << std::endl;
	std::cout << "* - host:            " << server_host << std::endl;
	std::cout << "* - port:            " << server_port << std::endl;
	std::cout << "* - password:        " << server_pass << std::endl;
	std::cout << "*" << std::endl;

	std::cout << "* Server infos:" << std::endl;
	std::cout << "* - name:            " << server_name << std::endl;
	std::cout << "* - token:           " << server_token << std::endl;
	std::cout << "* - max connections: " << server_max_connections << std::endl;
	std::cout << "* - info:            " << server_info << std::endl;
	std::cout << "*************************************************" << std::endl;

	Logger::setLogLevel(INFO);
	IRCServer server(server_port, server_pass, server_host);
	server.setName(server_name);
	server.setToken(server_token);
	server.setInfo(server_info);
	server.setMaxConnection(server_max_connections);
	if (has_network_connection)
		server.setNetworkConnection(network_host, network_port, network_pass);
	server.run();
	return (EXIT_SUCCESS);
}