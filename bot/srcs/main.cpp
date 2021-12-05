# include "ChanBot.hpp"

int main(int ac, char **av)
{
	if (ac < 4)
	{
		Logger::error("Usage: " + std::string(av[0]) + " host port password");
		return 1;
	}
	Logger::setLogLevel(DEBUG);
	std::string	network_host = av[1];
	std::string	network_pass = av[3];
	std::istringstream is(av[2]);
	ushort		network_port = 0;
	is >> network_port;
	if (network_port == 0)
	{
		Logger::error("Invalid port: " + std::string(av[2]));
		return 1;
	}
	ChanBot tony(network_host, network_port, network_pass);
	if (!tony.connect())
		return 1;
	tony.run();
	return 0;
}