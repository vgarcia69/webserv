#include "Server.hpp"
#include "Request.hpp"

void	addConnexion(int& fd, epoll_event& event, std::map<int, Client>& clients)
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
	epoll_ctl(Server::s_epoll_fd, EPOLL_CTL_ADD, socket, &event);

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
	clients[socket] = new_client;

	std::cout << "Client " << socket << " Added Successfully." << std::endl;
	std::cout << "Client IP Adress: " << new_client.getIPAdress() << std::endl;
	
}

void	removeConnexion(epoll_event& event, std::map<int, Client>& clients)
{
	epoll_ctl(Server::s_epoll_fd, EPOLL_CTL_DEL, event.data.fd, &event);
	close(event.data.fd);
	std::cout << "Removing Client: " << clients[event.data.fd].getSocketFD() << "." << std::endl;
	clients[event.data.fd].~Client();
}


void    handleClients(int& client_fd, epoll_event& event, std::map<int, Client>& clients)
{
    // removeConnexion(client_fd, event);
    Request request;

    (void)event;
	(void)clients;
    request.parsRequest(client_fd);
    // std::cout << request <<std::endl;
    request.handleRequest();

    std::string reponse = request.getHTTPreponse();
    // std::cout << "reponse is :\n" << reponse << std::endl;
    send(client_fd, reponse.c_str(), reponse.length(), 0);
}
