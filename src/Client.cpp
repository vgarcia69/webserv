#include "Client.hpp"

Client::Client(std::string ip_address, int port, int socket_fd)
:m_port(port), m_socket_fd(socket_fd), m_ip_address(ip_address)
{

}

Client::~Client()
{

}

int			Client::getPort()
{
	return m_port;
}

std::string	&Client::getIPAdress()
{
	return m_ip_address;
}

int			Client::getSocketFD()
{
	return m_socket_fd;
}

void		Client::setPort(int port)
{
	m_port = port;
}

void		Client::setIPAdress(std::string ip_address)
{
	m_ip_address = ip_address;
}

void		Client::setSocketFD(int socket_fd)
{
	m_socket_fd = socket_fd;
}

//-----------------------------------------------------------------------------------------Faire ou revoir la focntion	
bool Client::readSocket(int socket_fd, size_t max_size = 0) {
	//peut-etre gerer le cas d'une config avec un fichier plus petit que 4096 bit?
	std::vector<char> buffer(BUFFER_SIZE);
	int bytes_lus = 4096;
	
	std::cout << GREEN << "Going in" RESET << std::endl;
	while (bytes_lus == 4096) {
		bytes_lus = read(socket_fd, buffer.data(), BUFFER_SIZE);
		
		if (bytes_lus < 0) {
				throw std::runtime_error("Erreur lors de la lecture de la socket: " + 
									   std::string(strerror(errno)));
		} else if (bytes_lus == 0) {
			std::cout << YELLOW "essaie de deco ?" RESET << std::endl;
			return false;
		} else {
			if (max_size > 0 &&  m_processing_request.size() + bytes_lus > max_size + 2048)		//accept more byt for the header
			{
				//----------------------------------------------------------------------------------------gérer l'erreur 413 : fichier trop grand
				//renvoyer l'erreur + fermé la connexion
				break;
			}
			m_processing_request.append(buffer.data(), bytes_lus);
		}
	}
	if (m_processing_request.find("\r\n\r\n") != std::string::npos)
		return true;
	return false;
}

void			Client::clear()
{
	m_processing_request.clear();
	m_response.clear();
}
