#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(IProtocol & protocol) : SockStream(protocol), AEntity(), _protocol(&protocol)
{
	this->_init_commands();
}

Client::Client(SockStream &master) :  SockStream(master.getSocket(), master.getSockaddr(), *master.getProtocol()), AEntity(), _protocol(master.getProtocol())
{
	this->_init_commands();
}

Client::Client(int socket, const sockaddr_in &addr, IProtocol & protocol) : SockStream(socket, addr, protocol), _protocol(&protocol)
{
	this->_init_commands();
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
	this->close();
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client &				Client::operator=( Client const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
		(void) rhs;
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Client const & i )
{
	//o << "Value = " << i.getValue();
	(void) i;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

int					Client::execute(std::string data)
{
	size_t sep = data.find(" ");
	std::string command(data);
	std::string args("");
	if (sep < data.size())
	{
		command = data.substr(0, sep);
		args = data.substr(sep);
	}
	try {
		(this->_op.at(command))(*this, args);
	}
	catch (const std::out_of_range & e)
	{
		std::cout << "Error: unknow command\n";
		// std::cout << "Error: " << e.what() << std::endl;
	}
	return 1;
}

void				Client::_init_commands( void )
{
	this->_op.insert(std::make_pair("USER", &Client::USER));
	this->_op.insert(std::make_pair("PASS", &Client::PASS));
	this->_op.insert(std::make_pair("NICK", &Client::NICK));
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/*
** --------------------------------- COMMANDS ---------------------------------
*/

bool		Client::USER(Client & client, std::string cmd)
{
	std::cout << "<" << client.getSocket() << "> Command <USER> with args: " << cmd << std::endl;
	return true;
}

bool		Client::PASS(Client & client, std::string cmd)
{
	std::cout << "<" << client.getSocket() << "> Command <PASS> with args: " << cmd << std::endl;
	return true;
}

bool		Client::NICK(Client & client, std::string cmd)
{
	std::cout << "<" << client.getSocket() << "> Command <NICK> with args: " << cmd << std::endl;
	return true;
}

/* ************************************************************************** */