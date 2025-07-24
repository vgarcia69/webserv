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

void    handleRequest(int& client_fd, epoll_event& event, std::map<int, Client>& clients)
{
    // removeConnexion(client_fd, event);
    Request request;

    (void)event;
    request.parsRequest(client_fd);
    // std::cout << request <<std::endl;
    request.handleRequest();

    std::string reponse = request.getHTTPreponse();
	clients[client_fd].setRequest(request);
    // std::cout << "reponse is :\n" << reponse << std::endl;
    unsigned sent = send(client_fd, reponse.c_str(), reponse.length(), 0); // cut le content request de ce qui a ete envoyer

	// check si send a tout envoyer  si oui continue, sinon activer epollout 
	if (sent < reponse.length())
    	epoll_ctl(Server::s_epoll_fd, EPOLL_CTL_MOD, client_fd, &event);	
}

void	handleResponse(int& fd, epoll_event& event, std::map<int, Client>& clients) // recuperer le remaining
{
	(void)event; 
	std::cout << GREEN "HELLO ?" RESET << std::endl;
	if (!clients.count(fd))
	{
		std::cerr << "oulalala pas normal" << std::endl;
		return ;
	}
	Request		*request = &clients[fd].getRequest();
	std::string	response = request->getHTTPreponse();

    send(fd, response.c_str(), response.length(), 0);
}
