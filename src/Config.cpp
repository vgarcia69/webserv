#include "Config.hpp"

Config::Config()
{

}

Config::Config(const std::string& configFile)
{
	ParsingState	state = GLOBAL;
	std::string		currentString;
	std::fstream	configIn;

	configIn.open(configFile.c_str(), std::ios::in);
	if (!configIn.is_open())
		throw std::exception();

	while (1)
	{
		switch (state)
		{
			case GLOBAL:
			{
				state = lookingForServerCall(configIn, currentString);
				break ;
			}
			case LOOKING_FOR_SERVER_BLOCK: // get whole line
			{
				lookingForBracket(configIn, currentString);
				state = SERVER_BLOCK;
				break ;
			}
			case SERVER_BLOCK: // get line limited ;
			{
				// ParsingServerConfig();
			}
			case LOOKING_FOR_LOCATION_BLOCK: // get whole line
			{
				lookingForBracket(configIn, currentString);
				state = LOCATION_BLOCK;
				break ;
			}
			case LOCATION_BLOCK: // get line limited ;
			{
				// ParsingLocationConfig();
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
