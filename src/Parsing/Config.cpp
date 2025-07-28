#include "Config.hpp"

Config::Config(const std::string &configFile, std::vector<Server>& servers)
:m_servers(servers)
{
	Server				current_server;
	std::stringstream	content;
	std::fstream		configIn;
	m_state = GLOBAL;

	configIn.open(configFile.c_str(), std::ios::in);
	if (!configIn.is_open())
		throw std::runtime_error("Could not open the .conf file");

	setupContent(configIn, content);

	while (1)
	{
		switch (m_state)
		{
			case GLOBAL:
			{
				if (!lookingFor(content, "server") && m_servers.size())
					return ;
				if (!lookingFor(content, "{"))
					throw std::runtime_error(ERROR_BRACKET);
				m_servers.push_back(current_server);
				current_server.clear();
				m_state = SERVER_BLOCK;
			}
			case SERVER_BLOCK:
			{
				parsingServerInfo(content, m_state);
				break ;
			}
			case LOOKING_FOR_LOCATION_BLOCK:
			{
				if (!lookingFor(content, "{"))
					throw std::runtime_error(ERROR_BRACKET);
				m_state = LOCATION_BLOCK;
			}
			case LOCATION_BLOCK:
			{
				parsingLocationInfo(content, m_state);
				break ;
			}
			default:
			{
				throw std::runtime_error("Configuration File");
			}
		}
	}
}

Config::~Config()
{

}

void	Config::addLocationInfo(std::string keyword, std::string info)
{
	std::cout << "Adding info " BLUE << keyword << " " << info << RESET << std::endl;
	m_currentLoc.addInfo(keyword, info);
}

void	Config::addLocCGIPath(std::string& keyword)
{
	m_currentLoc.addCGIPath(keyword);
}

void	Config::addLocCGIExt(std::string& keyword)
{
	m_currentLoc.addCGIExt(keyword);
}
