#include "Server.hpp"

Server::Server()
{
	// remplacer le vide par un default config, check si existant ou quoi
	m_info.insert(m_info.end(), StringPair("server_name", "test"));
	m_info.insert(m_info.end(), StringPair("ipAddress", "127.0.0.1"));
	m_info.insert(m_info.end(), StringPair("port", "8001"));
	m_info.insert(m_info.end(), StringPair("root", ""));
	m_maxBodySize = 10;
	m_port = 8001;
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

	new_loc.addInfo("root", root);
	m_locations.push_back(new_loc);
}

void	Server::addInfo(std::string keyword, std::string& info)
{
	StringMap::iterator it = m_info.find(keyword);
	std::stringstream	seqInfo;

	seqInfo << info;
	info.clear();

	if (it != m_info.end())
	{
		seqInfo >> info;
		m_info[keyword] = info;
		// if (seqInfo) a finir
	}
	else
	{
		throw std::runtime_error("Invalid Argument in Configuration File");
	}
}
