#include "Config.hpp"

void	lookingFor(std::stringstream& content, std::string target, const char* errorMessage)
{
	std::string keyword;

	content >> keyword;
	if (keyword != target)
		throw std::runtime_error(errorMessage);
}

void	ParsingServerInfo(std::stringstream& sequenced_line, ParsingState& state, Server& server)
{
	std::string			keyWord;
	std::string			info;

	sequenced_line >> keyWord;
	if (keyWord == "location")
	{
		sequenced_line >> info;
		server.addLocation(info);
		state = LOOKING_FOR_LOCATION_BLOCK;
	}
	else 
	{
		std::getline(sequenced_line, info, ';');
		server.addInfo(keyWord, info);
	}
}

// void	ParsingLocationInfo(std::stringstream& sequencedLine, std::string& line, ParsingState& state, Server& server)
// {

// }

/*
ajouter des spacing entre chaque chars specials comme ; { } , 
puis reconvertir le tout en une stringstream big
*/
