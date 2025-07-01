#include "Server.hpp"

Server::Server()
{
	// remplacer le vide par un default config ?
	m_info.insert(m_info.end(), StringPair("server_name", ""));
	m_info.insert(m_info.end(), StringPair("ipAddress", ""));
	m_info.insert(m_info.end(), StringPair("port", ""));
	m_info.insert(m_info.end(), StringPair("root", ""));
	m_maxBodySize = 10;
}

Server::~Server()
{

}

void	Server::addLocation(std::string& root)
{
	Location newLoc;

	newLoc.addInfo("root", root);
	m_locations.push_back(newLoc);
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
		// if (seqInfo)
	}
	else
	{
		throw std::runtime_error("Invalid Argument in Configuration File");
	}
}
