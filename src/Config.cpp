#include "Config.hpp"

Config::Config()
{
	// find a default configuration ?
}

Config::Config(const std::string &configFile, Server& currentServer)
{
	ParsingState		state = GLOBAL;
	std::stringstream	content;
	std::fstream		configIn;

	configIn.open(configFile.c_str(), std::ios::in);
	if (!configIn.is_open())
		throw std::exception();

	content << configIn.rdbuf();

	while (1)
	{
		switch (state)
		{
			case GLOBAL:
			{
				lookingFor(content, "server", ERROR_SERVERBLOCK);
				lookingFor(content, "{", ERROR_BRACKET);
				//clear currentServer;
				state = SERVER_BLOCK;
			}
			case SERVER_BLOCK:
			{
				ParsingServerInfo(content, state, currentServer);
				break ;
			}
			case LOOKING_FOR_LOCATION_BLOCK:
			{
				//clear currentLocation
				// lookingForBracket(configIn, currentString);
				state = LOCATION_BLOCK;
			}
			case LOCATION_BLOCK: // get line limited ;
			{
				// ParsingLocationInfo(configIn, currentString, state, currentServer);
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
