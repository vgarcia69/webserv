#include "Server.hpp"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int	g_flag;

void	Server::addConnexion(int& fd, epoll_event& event)
{
	Client		new_client;
	sockaddr_in client_address;
	socklen_t	address_size;

	std::cout << "New Connexion Detected: " << fd << "." << std::endl;

	address_size = (socklen_t)sizeof(client_address);
	int socket = accept(fd, (sockaddr *)&client_address, &address_size);
	if (socket == -1)
		throw std::runtime_error("New Socket Failed to Init");
	
	event.events = EPOLLIN | EPOLLRDHUP;
	event.data.fd = socket;
	epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, socket, &event);

	int flags = fcntl(socket, F_GETFL);
	if (flags == -1)
		throw std::runtime_error("fcntl Failed");

	flags |= O_NONBLOCK;

	if (fcntl(socket, F_SETFL, flags) == -1)
		throw std::runtime_error("fcntl Failed");
	
	char inet_str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client_address.sin_addr), inet_str, INET_ADDRSTRLEN);

	new_client.setSocketFD(socket);
	new_client.setIPAdress(inet_str);
	m_clients.push_back(new_client);

	std::cout << "Client " << socket << " Added Successfully." << std::endl;
	std::cout << "Client IP Adress: " << new_client.getIPAdress() << std::endl;
	std::string message = "HELLOO";
	std::stringstream response;
	response << "HTTP/1.1 200 OK\r\n"
			<< "Content-Type: text/plain\r\n"
			<< "Content-Length: " << message.length() + 13 << "\r\n"
			<< "Connection: close\r\n"
			<< "\r\n"
			<< message <<  std::endl;

	std::string response_str = response.str();
	send(socket, response_str.c_str(), response_str.length(), 0);
	std::cout << BLUE << response.str() << RESET << std::endl;
}

void	Server::removeConnexion(int& fd, epoll_event& event)
{
	epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, &event);
	close(event.data.fd);
	std::cout << "Removing Client: " << fd << "." << std::endl;
}

void	Server::handleClients(int& client_fd, epoll_event& event)
{
    char	buffer[1024];
    int		bytes_read = read(client_fd, buffer, sizeof(buffer));

	buffer[bytes_read] = 0;
	if (bytes_read == -1)
	{
		std::cerr << "Error in Reading Client Request" << std::endl;
		removeConnexion(client_fd, event);
	}
	else if (bytes_read == 0)
    {
        removeConnexion(client_fd, event);
    }
    else
    {
		// server name dans la reponse maxsize aussi
        std::cout << buffer << std::endl;
		std::string message = "je fais un trux";
		std::stringstream response;
		response << "HTTP/1.1 200 OK\r\n"
				<< "Content-Type: text/plain\r\n"
				<< "Content-Length: " << message.length() + 13 << "\r\n"
				<< "Connection: close\r\n"
				<< "\r\n"
				<< message << std::endl;

		std::string response_str = response.str();
		send(client_fd, response_str.c_str(), response_str.length(), 0);
    }
}

int string_to_int(std::string host)
{
    unsigned int a, b, c, d;
    std::stringstream ss(host);
    char dot;

    ss >> a >> dot >> b >> dot >> c >> dot >> d;
    uint32_t ip = (a << 24) | (b << 16) | (c << 8) | d;

    return (htonl(ip));
}

void	Server::start()
{
	epoll_event	event;
	sockaddr_in	addr;

	addr.sin_port = htons(std::atoi(getInfo(PORT).c_str()));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = string_to_int(getInfo(HOST));

	m_epoll_fd = epoll_create1(0);
	if (m_epoll_fd == -1)
		throw std::runtime_error("Opening Epoll FD Failed");

	m_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_server_fd == -1)
		throw std::runtime_error("Opening Server Socket Failed"); 

	int reuse = 1;
	if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)))
		throw std::runtime_error("Server Socket Set Option Failed"); 

	if (bind(m_server_fd, (struct sockaddr *)&addr, sizeof(addr)))
	{
		perror("Error ");
		throw std::runtime_error("Binding Server Socket Failed");
	}

	if (listen(m_server_fd, SOMAXCONN))
		throw std::runtime_error("Listening Server Init Failed");

	g_flag = 0;
	event.events = EPOLLIN;
	event.data.fd = m_server_fd;
	epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_server_fd, &event);
}

void	Server::run()
{
	int			fd_num;
	epoll_event	events[SOMAXCONN];
	std::signal(SIGINT, shut);

	while (!g_flag)
	{
		fd_num = epoll_wait(m_epoll_fd, events, SOMAXCONN, -1);
		if (g_flag)
			break ;
		if (fd_num == -1)
		{
			std::cerr << "Poll Function Failed" << std::endl;
		}
		for (int i = 0; i < fd_num; i++)
		{
			if (events[i].data.fd == m_server_fd)
			{
				addConnexion(events[i].data.fd, events[i]);
			}
			else if (events[i].events & EPOLLRDHUP)
			{
				removeConnexion(events[i].data.fd, events[i]);
			}
			else
			{
				handleClients(events[i].data.fd, events[i]);
			}
		}
	}
	std::cout << YELLOW << "Closing file descriptors !" << RESET << std::endl;
	close(m_server_fd);
	close(m_epoll_fd);
}

void	Server::shut(int)
{
	std::cout << YELLOW << "Shutting down the Server" << RESET << std::endl;
	g_flag = 1;
}
