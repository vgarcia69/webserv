#include "Server.hpp"
#include "Request.hpp"

void	addConnexion(int& fd, epoll_event& event, std::map<int, Client>& clients)
{
	sockaddr_in client_address;
	socklen_t	address_size;

	std::cout << "New Connexion Detected: " << fd << "." << std::endl;

	address_size = (socklen_t)sizeof(client_address);
	int socket = accept(fd, (sockaddr *)&client_address, &address_size);
	if (socket == -1)
		throw std::runtime_error("New Socket Failed to Init");
	
	event.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
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

	clients[socket].setSocketFD(socket);
	clients[socket].setIPAdress(inet_str);

	std::cout << "Client " << socket << " Added Successfully." << std::endl;
	std::cout << "Client IP Adress: " << clients[socket].getIPAdress() << std::endl;
}

void	removeConnexion(epoll_event& event, std::map<int, Client>& clients)
{
	epoll_ctl(Server::s_epoll_fd, EPOLL_CTL_DEL, event.data.fd, &event);
	close(event.data.fd);
	std::cout << "Removing Client: " << clients[event.data.fd].getSocketFD() << "." << std::endl;
	clients.erase(event.data.fd);
}

void	handleRequest(int& client_fd, epoll_event& event, std::map<int, Client>& clients)
{
	if (!clients[client_fd].readSocket(client_fd, 0))
	{
		//-----------------------------------------pourquoi tu remove la connexion??
		removeConnexion(event, clients);
		return ;
	}
	
	std::cout << clients[client_fd].getProcessRequest() <<std::endl;
	if (clients[client_fd].request.getNotEnd()){
		clients[client_fd].request.addBody(clients[client_fd].getProcessRequest());
	}
	else {
		clients[client_fd].request.parsRequest(clients[client_fd].getProcessRequest());
	}
	if (clients[client_fd].request.getNotEnd()){
		return ;
	}
	// std::cout << clients[client_fd].request << std::endl;
	clients[client_fd].request.handleRequest();

	clients[client_fd].m_response = clients[client_fd].request.getHTTPresponse();
	std::cout << "reponse is :\n" << clients[client_fd].m_response << std::endl;
	unsigned sent = send(client_fd, clients[client_fd].m_response.c_str(), clients[client_fd].m_response.length(), 0);

	if (sent < clients[client_fd].m_response.length())
	{
		clients[client_fd].m_response.erase(0, sent);
  		if (epoll_ctl(Server::s_epoll_fd, EPOLL_CTL_MOD, client_fd, &event))
			throw std::runtime_error ("epoll_ctl failed");
	}
	else
		clients[client_fd].clear();
}

void	handleResponse(int& fd, epoll_event& event, std::map<int, Client>& clients)
{
	unsigned sent = send(fd, clients[fd].m_response.c_str(), clients[fd].m_response.length(), 0);
	if (sent < clients[fd].m_response.length())
	{
		clients[fd].m_response.erase(0, sent);
  		if (epoll_ctl(Server::s_epoll_fd, EPOLL_CTL_MOD, fd, &event))
			throw std::runtime_error ("epoll_ctl failed");
	}
	else
		clients[fd].clear();
}
