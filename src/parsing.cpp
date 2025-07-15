#include "Config.hpp"

void	lookingFor(std::stringstream& content, std::string target, const char* errorMessage)
{
	std::string keyword;

	content >> keyword;
	if (keyword != target)
		throw std::runtime_error(errorMessage);
}

void	Config::ParsingServerInfo(std::stringstream& sequenced_line, ParsingState& state, Server& server)
{
	std::string			keyword;
	std::string			info;

	sequenced_line >> keyword;
	if (keyword == "location")
	{
		sequenced_line >> info;
		server.addLocation(info);
		state = LOOKING_FOR_LOCATION_BLOCK;
	}
	else if (keyword == "listen")
	{
		sequenced_line >> info;
		parsingIPAddress(info);
	}
	else 
	{
		std::getline(sequenced_line, info, ';');
		server.addInfo(keyword, info);
	}
}

// void	Config::ParsingLocationInfo(std::stringstream& content, ParsingState& state, Server& server)
// {

// }

void	setupContent(std::fstream& configIn, std::stringstream& content)
{
	std::string			file;
	size_t				pos = 0;

	getline(configIn, file, '\0');
	if (!file.length())
		throw std::runtime_error("Empty config file");
	while (pos < file.size())
	{
		pos = file.find_first_of("{};#", pos + 1);
		if (pos == std::string::npos)
			break;
		else if (file[pos] == '#')
		{
			size_t	endline_pos = file.find_first_of("\n", pos);

			if (endline_pos != std::string::npos)
				file.erase(pos, endline_pos - pos);
		}
		else
		{
			file.insert(pos, " ");
			pos++;
		}
	}

	std::cout << file <<std::endl;
	content << file;
}

void	Config::parsingIPAddress(std::string& address)
{
	int			index = 0;
	int			checker;
	std::string	format = "XXX.XXX.XXX.XXX";
	std::string buffer;

	for (int i = 0; ((isdigit(address[i]) && format[i] == 'X') || format[i] == address[i]); i++)
	{
		if (format[i] == '.' || !format[i])
		{
			checker = std::atoi(buffer.c_str());
			std::cout << checker << std::endl;
			if (1 > checker || checker > 255)
				throw std::runtime_error("Invalid IP Adress");
			if (index == 3)
			{
				m_servers.back().addInfo("listen", address);
				return ;
			}
			buffer.clear();
			index++;
			continue;
		}
		buffer += address[i];
	}
	throw std::runtime_error("invalid");
}
