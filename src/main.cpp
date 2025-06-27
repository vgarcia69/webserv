#include "Config.hpp"

int main(int ac, char **av)
{
	if (ac > 2)
	{
		// write something
		return 1;
	}

	// parse config
	try{
	Config config(av[1]);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	// init server with parsed config

	// run server

	return 0;
}
