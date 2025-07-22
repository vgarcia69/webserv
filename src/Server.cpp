#include "Server.hpp"

int	Server::s_nb_servers_running = 0;
int	Server::s_epoll_fd = 0;

Server::Server()
{
	m_server_fd = 0;
}

Server::~Server()
{
	for (unsigned i = 0; i < m_clients.size(); i++)
		close(m_clients[i].getSocketFD());

	close(Server::s_epoll_fd);
	close(m_server_fd);
}

void	Server::addLocation(std::string& root)
{
	Location new_loc;

	new_loc.addInfo(ROOT, root);
	m_locations.push_back(new_loc);
}

void	Server::addInfo(std::string keyword, std::string info)
{
	std::cout << "adding: "<< keyword << " " << info << std::endl;
	m_info[keyword] = info;
}

std::string	Server::getInfo(std::string keyword)
{
	if (m_info.count(keyword))
		return m_info[keyword];
	return NOT_FOUND;
}

void		Server::addErrorPage(int nbr, std::string path)
{
	m_errorPages[nbr] = path;
}

std::string	Server::getErrorPage(int nbr)
{
	return m_errorPages[nbr];
}

void	Server::addLocationInfo(std::string keyword, std::string info)
{
	m_locations.back().addInfo(keyword, info);
}

std::string	Server::getLocationInfo(std::string keyword)
{
	return m_locations.back().getInfo(keyword);
}

int			Server::getServerFD()
{
	return m_server_fd;
}

static int string_to_int(std::string host)
{
    unsigned int a, b, c, d;
    std::stringstream ss(host);
    char dot;

    ss >> a >> dot >> b >> dot >> c >> dot >> d;
    uint32_t ip = (a << 24) | (b << 16) | (c << 8) | d;

    return (htonl(ip));
}

bool Server::init()
{
	epoll_event	event;
	sockaddr_in	addr;

	addr.sin_port = htons(std::atoi(getInfo(PORT).c_str()));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = string_to_int(getInfo(HOST));


	m_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_server_fd == -1)
	{
		std::cerr << "Opening Server Socket Failed" << std::endl; 
		return 1;
	}

	int reuse = 1;
	if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)))
	{
		std::cerr << "Server Socket Set Option Failed" << std::endl; 
		return 1;
	}

	if (bind(m_server_fd, (struct sockaddr *)&addr, sizeof(addr)))
	{
		std::cerr << "Binding Server Socket Failed" << std::endl;
		return 1;
	}

	if (listen(m_server_fd, SOMAXCONN))
	{
		std::cerr << "Listening Server Init Failed" << std::endl;
		return 1;
	}

	event.events = EPOLLIN;
	event.data.fd = m_server_fd;
	epoll_ctl(s_epoll_fd, EPOLL_CTL_ADD, m_server_fd, &event);
	return 0;
}
