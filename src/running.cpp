#include "Config.hpp"
#include "Server.hpp"

int g_flag;

void	startServers(std::vector<Server>& servers)
{
	Server::s_epoll_fd = epoll_create1(0);
	if (Server::s_epoll_fd == -1)
		throw	std::runtime_error("Epoll create Failed");

	for (unsigned i = 0; i < servers.size(); i++)
	{
		if (!servers[i].init())
			Server::s_nb_servers_running++;
	}
	if (!Server::s_nb_servers_running)
		throw std::runtime_error("All the servers failed to start");
	g_flag = 0;
}

void	shut(int)
{
	std::cout << YELLOW << "Shutting down the Servers" << RESET << std::endl;
	g_flag = 1;
}

static bool	isNewEntrance(int fd, std::vector<Server>& servers)
{
	for (unsigned i = 0; i < servers.size(); i++)
	{
		if (fd == servers[i].getServerFD())
			return 1;
	}
	return 0;
}

void	runServers(std::vector<Server>& servers)
{
	std::map<int, Client>	clients;
	epoll_event				events[SOMAXCONN];
	int						fd_num;

	std::signal(SIGINT, shut);

	while (!g_flag)
	{
		fd_num = epoll_wait(Server::s_epoll_fd, events, SOMAXCONN, -1);
		if (fd_num == -1)
		{
			std::cerr << "EPoll Function Cancelled" << std::endl;
		}
		for (int i = 0; i < fd_num; i++)
		{
			if (isNewEntrance(events[i].data.fd, servers))
			{
				std::cout << YELLOW "Adding new client" RESET << std::endl;
				addConnexion(events[i].data.fd, events[i], clients);
			}
			else if (events[i].events & EPOLLRDHUP)
			{
				std::cout << YELLOW "Removing client" RESET << std::endl;
				removeConnexion(events[i], clients);
			}
			else
			{
				std::cout << YELLOW "Handle client Request" RESET << std::endl;
				handleClients(events[i].data.fd, events[i], clients);
			}
		}
	}
}
