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
		Config config(av[1]);
		for (unsigned i = 0; i < config.m_servers.size(); i++)
		{
			runServer(config.m_servers[i]);
		}
		wait(NULL);
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
