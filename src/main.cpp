#include "Config.hpp"
#include "Server.hpp"

static void	runServer(Server& server);

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
		std::string test = "default.conf";
		if (av[1] != NULL)
			test = av[1];
		Config config(test, servers);

	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;
}

static void	runServer(Server& server)
{
	int pid;

	pid = fork();
	if (pid == -1)
		std::cerr << "something" << std::endl;
	else if (!pid)
	{
		server.start();
		server.run();
	}
}
