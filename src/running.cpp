#include "Server.hpp"

void	Server::addConnexion(int& fd, epoll_event& event)
{
	// creer aussi une socket pour server_fd ?
	epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event);
}

void	Server::removeConnexion(int& fd, epoll_event& event)
{
	epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, &event);
}

void	Server::handleClients(int& fd)
{
	// ici on va cheque les request et les envoyés au parsing de req'
	(void)fd;
}

// epoll est set pour read les info de chaque socket, server_fd est setup pour l ecoute de nouvelles connexions
void	Server::start()
{
	sockaddr_in	addr;
	addr.sin_port = htons(m_port);

	m_epoll_fd = epoll_create1(EPOLL_CLOEXEC);
	m_server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (m_server_fd == -1)
		throw std::runtime_error("Opening Server Socket Failed"); 

	if (bind(m_server_fd, (sockaddr *)&addr, sizeof(addr)) == -1)
		throw std::runtime_error("Binding Server Socket Failed"); 

	if (listen(m_server_fd, SOMAXCONN))
		throw std::runtime_error("Listening Server Init Failed");
}

void	Server::run()
{
	int	fd_num;
	epoll_event	events[SOMAXCONN]; 

	while (1)
	{
		fd_num = epoll_wait(m_epoll_fd, events, SOMAXCONN, -1);
		if (fd_num == -1)
		{
			std::cerr << "Poll Function Failed" << std::endl;
		}
		for (unsigned i = 0; i < fd_num; i++)
		{
			if (events[i].data.fd == m_epoll_fd)
			{
				addConnexion(events[i].data.fd, events[i]);
			}
			else if (events[i].events == EPOLLRDHUP)
			{
				removeConnexion(events[i].data.fd, events[i]);
			}
			else
			{
				handleClients(events[i].data.fd);
			}
		}
	}
}

void	shutdown()
{

}
