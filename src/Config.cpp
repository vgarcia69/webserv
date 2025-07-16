#include "Config.hpp"

Config::Config()
{
	// find a default configuration ?
}

Config::Config(const std::string &configFile, Server& currentServer)
{
	m_state = GLOBAL;
	std::stringstream	content;
	std::fstream		configIn;

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
				if (!lookingFor(content, "server"))
					break ;
				if (!lookingFor(content, "{"))
					throw std::runtime_error(ERROR_BRACKET);
				//clear currentServer;
				m_state = SERVER_BLOCK;
			}
			case SERVER_BLOCK:
			{
				parsingServerInfo(content, m_state, currentServer);
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
				parsingLocationInfo(content, m_state, currentServer);
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
