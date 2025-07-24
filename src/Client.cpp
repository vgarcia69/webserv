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

bool Client::readSocket(int socket_fd, size_t max_size = 0) {
	const size_t BUFFER_SIZE = 4096;
	std::vector<char> buffer(BUFFER_SIZE);
	
	while (true) {
		ssize_t bytes_lus = read(socket_fd, buffer.data(), BUFFER_SIZE);
		
		if (bytes_lus < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				// Socket non-bloquante, pas plus de données disponibles
				break;
			} else {
				throw std::runtime_error("Erreur lors de la lecture de la socket: " + 
									   std::string(strerror(errno)));
			}
		} else if (bytes_lus == 0) {
			// Connexion fermée par le peer
			std::cout << YELLOW "hello" RESET << std::endl;
			break;
		} else {
			// Vérifier la limite de taille si spécifiée
			if (max_size > 0 && m_processing_request.size() + bytes_lus > max_size) {
				size_t bytes_a_ajouter = max_size - m_processing_request.size();
				m_processing_request.append(buffer.data(), bytes_a_ajouter);
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
