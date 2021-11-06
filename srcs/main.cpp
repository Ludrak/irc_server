#include "IRCServer.hpp"

#include <exception>

int		Usage( const char * executable )
{
	Logger::info("Usage:");
	Logger::info(std::string("\t") + executable + " [host:port_network:password_network] <port> <password>");
	return 1;
}

//TODO see/add functions allowed by subject

int main(int ac, char ** av)
{
	int			port = 0;
	int			port_network = 0;
	std::string	host("");
	std::string	password("");
	std::string	password_network("");

	// Logger::lo
	if (ac < 3 || ac > 4)
		return Usage(av[0]);
	else if (ac == 3)
	{
		port = atoi(av[1]);
	 	password = av[2];
	}
	else
	{
		std::string config(av[1]);
		size_t sep1 = config.find(':');
		size_t sep2 = config.find(':', sep1 + 1);
		if (sep1 == std::string::npos || sep2 == std::string::npos)
			return Usage(av[0]);
		host = config.substr(0, sep1);
		if (host.size() == 0
		|| config.substr(sep1 + 1, sep2 - sep1 - 1).size() == 0)
			return Usage(av[0]);
		try {
			port_network = stoi(config.substr(sep1 + 1, sep2 - sep1 - 1));
		}
		catch (const std::invalid_argument & e)
		{
			Logger::error("Invalid network port specified");
			return Usage(av[0]);
		}
		password_network = config.substr(sep2 + 1);
		port = atoi(av[2]);
	 	password = std::string(av[3]);
	}
	if (port == 0)
	{
		Logger::error("Invalid port specified");
		return Usage(av[0]);
	}
	try 
	{
		IRCServer server(port, password, "127.0.0.1");
		if (!server.setNetworkConnection(host, port_network, password_network))
		{
			Logger::error("Bad network params");
			return Usage(av[0]);
		}
		server.run();
	}
	catch (std::exception &e)
	{
		Logger::critical(e.what());
	}
    return (0);
}