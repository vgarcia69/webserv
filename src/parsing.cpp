#include "Config.hpp"

void	lookingFor(std::stringstream& content, std::string target, const char* errorMessage)
{
	std::string keyWord;

	content >> keyWord;
	if (keyWord != target)
		throw std::runtime_error(errorMessage);
}

void	ParsingServerInfo(std::stringstream& sequencedLine, ParsingState& state, Server& server)
{
	std::string			keyWord;
	std::string			info;

	sequencedLine >> keyWord;
	if (keyWord == "location")
	{
		sequencedLine >> info;
		server.addLocation(info);
		state = LOOKING_FOR_LOCATION_BLOCK;
	}
	else 
	{
		std::getline(sequencedLine, info, ';');
		server.addInfo(keyWord, info);
	}
}

// void	ParsingLocationInfo(std::stringstream& sequencedLine, std::string& line, ParsingState& state, Server& server)
// {

// }
