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
	if (keyword == "}")
	{
		state = SERVER_BLOCK;
		return ;
	}
	sequenced_line >> keyword;
	if (keyword != END_INSTRUC)
		throw std::runtime_error("Instruction must be followed by a ;");
}

void	Config::parseLocDefaultFile(std::string& path)
{
	std::string	root;

	root = m_servers.back().getLocationInfo(ROOT);
	if (root == NOT_FOUND)
	{
	    root = m_servers.back().getInfo(ROOT);
	    if (root == NOT_FOUND)
	    	root.clear();
	}
	root += path;

	if (access(root.c_str(), F_OK | R_OK))
		throw std::runtime_error("Invalid Error Page Path");
	m_servers.back().addLocationInfo(ROOT, root);
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
		m_servers.back().addLocationInfo(keyword, keyword);
	}
}

void	Config::parseAutoIndex(std::string info)
{
	if (info == "on" || info == "off")
		m_servers.back().addLocationInfo(AUTOINDEX, info);
	else
		throw std::runtime_error("Invalid Autoindex Syntax");
}

void	Config::parseReturn(std::string info)
{
	std::string	root;

	root = m_servers.back().getLocationInfo(ROOT);
	if (root == NOT_FOUND)
	{
	    root = m_servers.back().getInfo(ROOT);
	    if (root == NOT_FOUND)
	    	root.clear();
	}
	root += info;
	std::cout << root << std::endl;
	if (access(root.c_str(), F_OK | R_OK))
		throw std::runtime_error("Invalid Return Redirection");
	m_servers.back().addLocationInfo(RETURN, root);
}
