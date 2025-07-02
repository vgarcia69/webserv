#include "Server.hpp"

void	Server::addConnexion(int& fd, epoll_event& event)
{
	// creer aussi une socket pour server_fd ?
	std::cout << "New Connexion Detected: " << fd << "." << std::endl;
	epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event);
	int socket = accept(fd, NULL, NULL);
	if (socket == -1)
		throw std::runtime_error("New Socket Failed to Init");
	
	int flags = fcntl(socket, F_GETFL);
	if (flags == -1)
		throw std::runtime_error("fcntl Failed");

	flags |= O_NONBLOCK; // ajouter nonblock flag aux flags de la socket

	if (fcntl(socket, F_SETFL, flags) == -1)
		throw std::runtime_error("fcntl Failed");
	// ptet creer un client connecter a la socket. dans une map de clients ?
	std::cout << "Client " << fd << " Added Successfully." << std::endl;
}

void	Server::removeConnexion(int& fd, epoll_event& event)
{
	epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, &event);
	std::cout << "Removing Client: " << fd << "." << std::endl;
}

void	Server::handleClients(int& fd)
{
	// ici on va cheque les request et les envoyés au parsing
	std::cout << "Handling Client " << fd <<  " Request." << std::endl;
}

// epoll est set pour read les info de chaque socket, server_fd est setup pour l ecoute de nouvelles connexions
void	Server::start()
{
	sockaddr_in	addr; // type pour injecter le port pour bind le server_fd
	addr.sin_port = htons(m_port); // htons convert int16 host byte to int16 network byte

	m_epoll_fd = epoll_create1(0); 
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
	int fd_num;
	epoll_event	events[SOMAXCONN]; 
	std::signal(SIGINT, shut);

	addConnexion(m_server_fd, events[0]);
	while (1)
	{
		fd_num = epoll_wait(m_epoll_fd, events, SOMAXCONN, -1);
		std::cout << "helloo" <<std::endl;
		if (fd_num == -1)
		{
			std::cerr << "Poll Function Failed" << std::endl;
		}
		for (int i = 0; i < fd_num; i++)
		{
			if (events[i].data.fd == m_epoll_fd)
			{
				addConnexion(events[i].data.fd, events[i]);
				std::cout << "fd_num:" << fd_num << " et i: " << i << std::endl;
			}
			else if (events[i].events == EPOLLRDHUP)
			{
				removeConnexion(events[i].data.fd, events[i]);
				std::cout << "j attends quelque chose" << std::endl;
			}
			else
			{
				handleClients(events[i].data.fd);
				std::cout << "je me cache ici" << std::endl;
			}
		}
		std::cout << "j attends quelque chose" << std::endl;
	}
}

void	Server::shut(int)
{
	throw SafeExit();
}
