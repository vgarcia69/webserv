#include "Config.hpp"

void	Config::parsingLocationInfo(std::stringstream& sequenced_line, ParsingState& state, Server& server)
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
		sequenced_line >> keyword;
		parseReturn(info, keyword);
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
