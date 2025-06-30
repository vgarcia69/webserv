#include "Config.hpp"

int main(int ac, char **av)
{
	if (ac > 2)
	{
		std::cerr << "Wrong Amount of Args" << std::endl;
		return 1;
	}

	Server server;

	try
	{
		Config config(av[1], server);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	server.run();

	return 0;
}
