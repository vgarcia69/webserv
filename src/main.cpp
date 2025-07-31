#include "Config.hpp"
#include "Server.hpp"

void	runServers(std::vector<Server>& servers);
void	startServers(std::vector<Server>& servers);

int main(int ac, char **av)
{
	if (ac > 2)
	{
		std::cerr << "Wrong Amount of Args" << std::endl;
		return 1;
	}

	try
	{
		std::vector<Server> servers;
		std::string			config_file = "default.conf";

		if (av[1] != NULL)
			config_file = av[1];

		Config config(config_file, servers);

		startServers(servers);
		runServers(servers);
	}
	catch (std::exception &e)
	{
		std::cout << YELLOW << std::endl;
		std::cout << e.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cout << BLUE << std::endl;
		return 0;
	}
	std::cout << RED << std::endl;
	return 0;
}
