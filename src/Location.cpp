#include "Location.hpp"

Location::Location()
{
	m_info.insert(m_info.end(), StringPair("root", ""));	
	m_info.insert(m_info.end(), StringPair("allow_methods", ""));	
	m_info.insert(m_info.end(), StringPair("redirection", ""));	
	m_info.insert(m_info.end(), StringPair("upload_dir", ""));	
	m_info.insert(m_info.end(), StringPair("autoindex", "on"));	
	m_info.insert(m_info.end(), StringPair("", ""));	

}

Location::~Location()
{

}

void	Location::addInfo(std::string keyword, std::string info)
{
	// StringMap::iterator it = m_info.find(keyword);

	m_info[keyword] = info;
	// if (it != m_info.end())
	// {
	// 	m_info[keyword] = info;
	// }
	// else
	// {
	// 	throw std::runtime_error("Invalid Argument in Configuration Location");
	// }
}

void	Location::addCGI(std::string& suffix, std::string& info)
{
	m_cgi = StringPair(suffix, info);
	// a voir ajouter une pair avec le keyword extension
}

std::string	Location::getInfo(std::string keyword)
{
	if (m_info.count(keyword))
		return m_info[keyword];
	return NOT_FOUND;
}
