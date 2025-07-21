#include "Server.hpp"

Server::Server()
{
	// remplacer le vide par un default config, check si existant ou quoi
	// m_info.insert(m_info.end(), StringPair("server_name", "test"));
	// m_info.insert(m_info.end(), StringPair("host", "127.0.0.1"));
	// m_info.insert(m_info.end(), StringPair("port", "8001"));
	// m_info.insert(m_info.end(), StringPair("root", ""));
	// m_info.insert(m_info.end(), StringPair("client_max_body_size", "1024"));

	m_epoll_fd = 0;
	m_server_fd = 0;
}

Server::~Server()
{
	for (unsigned i = 0; i < m_clients.size(); i++)
		close(m_clients[i].getSocketFD());

	close(m_epoll_fd);
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
	// StringMap::iterator it = m_info.find(keyword);

	std::cout << "adding: "<< keyword << " " << info << std::endl;
	m_info[keyword] = info;
	// if (it == m_info.end())
	// {
	// 	std::cout << "adding info: "<< keyword << " " << info << std::endl;
	// 	m_info[keyword] = info;
	// }
	// else
	// {
	// 	std::cout << "info: " << m_info[keyword] << std::endl;
	// 	std::cout << "new info: "<< keyword << " " << info << std::endl;
	// 	throw std::runtime_error("Argument Already Set up");
	// }
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
