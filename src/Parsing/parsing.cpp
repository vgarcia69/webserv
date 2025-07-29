#include "Config.hpp"

void	Config::parsingServerInfo(std::stringstream& sequenced_line, ParsingState& state)
{
	std::string			keyword;
	std::string			info;

	sequenced_line >> keyword;
	if (keyword == LOCATION)
	{
		sequenced_line >> info;
		info.insert(0, 1, '.');
		std::string root = m_servers.back().getInfo(ROOT);
		if (root == NOT_FOUND)
			root.clear();
		root += info;

		std::cout << root << std::endl;
		if (!isDirectory(root))
			throw std::runtime_error("Invalid Location Path");
		m_servers.back().addLocation(m_currentLoc, root);
		std::cout << "Adding " GREEN << root << RESET << std::endl;
		m_currentLoc.clear();
		state = LOOKING_FOR_LOCATION_BLOCK;
		return ;
	}
	if (keyword == "}")
	{
		state = GLOBAL;
		return ;
	}
	sequenced_line >> info;
	if (keyword == HOST)
		parsingIPAddress(info);
	else if (keyword == PORT)
		parsingPort(info);
	else if (keyword == SERVER_NAME) //need to be added into /etc/hosts to work
		parsingServerName(info);
	else if (keyword == MAX_SIZE)
		parsingMaxBodySize(info);
	else if (keyword == DEFAULT_FILE)
		parseDefaultFile(info);
	else if (keyword == ERROR_PAGE)
	{
		sequenced_line >> keyword;
		parseErrorPage(info, keyword);
	}
	else if (keyword == ROOT)
	{
		checkRoot(info);
		m_servers.back().addInfo(ROOT, info);
	}
	else
	{
		std::cerr << "Instruct: "<< keyword  << " " << info << std::endl;
		throw std::runtime_error("Unknown instruction");
	}
	
	sequenced_line >> keyword;
	if (keyword != END_INSTRUC)
		throw std::runtime_error("Instruction must be followed by a ;");
}

bool	lookingFor(std::stringstream& content, std::string target)
{
	std::string keyword;

	content >> keyword;
	if (keyword == target)
		return true;
	return false;
}

void	setupContent(std::fstream& configIn, std::stringstream& content)
{
	std::string	file;
	size_t		pos = 0;

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
			file.insert(pos + 1, " ");
			file.insert(pos, " ");
			pos++;
		}
	}
	content << file;
}
