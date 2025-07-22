#include "Server.hpp"

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

void	removeConnexion(epoll_event& event, std::map<int, Client>& clients)
{
	epoll_ctl(Server::s_epoll_fd, EPOLL_CTL_DEL, event.data.fd, &event);
	close(event.data.fd);
	std::cout << "Removing Client: " << clients[event.data.fd].getSocketFD() << "." << std::endl;
	clients[event.data.fd].~Client();
}

void	handleClients(int& client_fd, epoll_event& event, std::map<int, Client>& clients)
{
    char	buffer[1024];
    int		bytes_read = read(client_fd, buffer, sizeof(buffer));

	buffer[bytes_read] = 0;
	if (bytes_read == -1)
	{
		std::cerr << "Error in Reading Client Request" << std::endl;
		removeConnexion(event, clients);
	}
	else if (bytes_read == 0)
    {
        removeConnexion(event, clients);
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

