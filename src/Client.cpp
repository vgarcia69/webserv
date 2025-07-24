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


Request&			Client::getRequest()
{
	return *m_request;
}

void		Client::setRequest(Request& request)
{
	m_request = &request;
}
