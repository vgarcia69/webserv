#include "Config.hpp"

void	Config::parsingLocationInfo(std::stringstream& sequenced_line, ParsingState& state)
{
	std::string			keyword;
	std::string			info;

	sequenced_line >> keyword;
	if (keyword == "}")
	{
		state = SERVER_BLOCK;
		return ;
	}
	else if (keyword == METHODS)
	{
		parseMethods(sequenced_line);
		return ;
	}
	sequenced_line >> info;
	if (keyword == ROOT)
	{
		checkRoot(info);
	}
	else if (keyword == AUTOINDEX)
	{
		parseAutoIndex(info);
	}
	else if (keyword == RETURN)
	{
		parseReturn(info);
	}
	else if (keyword == "}")
	{
		state = SERVER_BLOCK;
		m_servers.back().addLocation(m_currentLoc, m_currentLoc.m_root);
		return ;
	}
	else if (keyword == DEFAULT_FILE)
	{
		parseLocDefaultFile(info);
	}
	else
	{
		std::cerr << "Instruct: "<< keyword  << " " << info << std::endl;
		throw std::runtime_error("Unknown Location instruction");
	}
	sequenced_line >> keyword;
	if (keyword != END_INSTRUC)
		throw std::runtime_error("Instruction must be followed by a ;");
}

void	Config::parseLocDefaultFile(std::string& path)
{
	std::string	file_path;

	file_path = m_servers.back().getLocationInfoOf(ROOT, m_currentLoc);
	if (file_path == NOT_FOUND)
	{
	    file_path = m_servers.back().getInfo(ROOT);
	    if (file_path == NOT_FOUND)
	    	file_path.clear();
	}
	file_path += path;

	if (access(file_path.c_str(), F_OK | R_OK))
		throw std::runtime_error("Invalid Error Page Path");
	addLocationInfo(DEFAULT_FILE, file_path);
}

void	Config::parseMethods(std::stringstream& sequenced_line)
{
	std::string keyword;

	while (!sequenced_line.fail())
	{
		sequenced_line >> keyword;
		if (keyword != POST && keyword != GET && keyword != DELETE)
		{
			if (keyword != END_INSTRUC)
				throw std::runtime_error("Invalid Syntax Method");
			break ;
		}
		addLocationInfo(keyword, keyword);
	}
}

void	Config::parseAutoIndex(std::string info)
{
	if (info == "on" || info == "off")
		addLocationInfo(AUTOINDEX, info);
	else
		throw std::runtime_error("Invalid Autoindex Syntax");
}

void	Config::parseReturn(std::string info)
{
	std::string	root;

	root = m_servers.back().getLocationInfoOf(ROOT, m_currentLoc);
	if (root == NOT_FOUND)
	{
	    root = m_servers.back().getInfo(ROOT);
	    if (root == NOT_FOUND)
	    	root.clear();
	}
	root += info;
	// std::cerr << root <<" info: "<< info <<  std::endl;
	if (access(root.c_str(), F_OK | R_OK))
		throw std::runtime_error("Invalid Return Redirection");
	addLocationInfo(RETURN, root);
}
