#include "Config.hpp"

static setFlagFile()
{
	open("/flag", O_CREAT, O_)
}

Config::Config()
{
	// find a default configuration ?
}

Config::Config(const std::string &configFile)
{
	int					flag_fd = setFlagFile();
	Server				current_server(flag_fd);
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
				//clear currentServer;
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
			case LOCATION_BLOCK: // get line limited ;
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
