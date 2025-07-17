#include "Config.hpp"

void	Config::parseDefaultFile(std::string& path)
{
	std::string	root;

	root = m_servers.back().getInfo(ROOT);
	if (root == NOT_FOUND)
	  	root.clear();
	root += path;

	// std::cout << YELLOW << root << RESET << std::endl;

	if (access(root.c_str(), F_OK | R_OK))
		throw std::runtime_error("Invalid Error File Path");
	m_servers.back().addInfo(DEFAULT_FILE, root);
}

void	Config::parsingPort(std::string& info)
{
	std::string			port_str;
	int					port;
	size_t				index;
	
	index = info.find(":");
	if (index != std::string::npos)
	{
		port_str = info.substr(index + 1);
		info.erase(index);
		parsingIPAddress(info);
	}
	else
		port_str = info;

	std::stringstream	ss(port_str);
	ss >> port;

	if (ss.fail() || !ss.eof() || port < 1 || port > 65535 || port_str.size() != 4)
		throw std::runtime_error("Invalid Port Parsed");
	else
		m_servers.back().addInfo(PORT, port_str);
}

void	Config::parsingIPAddress(std::string address)
{
	std::string buffer;
	int			checker;

	if (address == "localhost")
	{
		m_servers.back().addInfo(HOST, "127.0.0.1");
		return ;
	}

	// std::cout << YELLOW << address << RESET << std::endl;

	for (unsigned i = 0, index = 0; isdigit(address[i]) || address[i] == '.' || !address[i]; i++)
	{
		if (address[i] == '.' || !address[i])
		{
			checker = std::atoi(buffer.c_str());
			if (0 > checker || checker > 255)
				throw std::runtime_error("Invalid IP Address number");
			if (index == 3)
			{
				m_servers.back().addInfo(HOST, address);
				return ;
			}
			buffer.clear();
			index++;
			continue;
		}
		buffer += address[i];
	}
	std::cout << address << std::endl;
	throw std::runtime_error("Invalid IP Address");
}

void	Config::parsingServerName(std::string& info)
{
	for (unsigned i = 0; i < info.size(); i++)
	{
        if (!std::isalnum(info[i]))
            throw std::runtime_error("Invalid Server Name, needs to be alpha-numeric");
    }
	m_servers.back().addInfo(SERVER_NAME, info);
}

void	Config::parsingMaxBodySize(std::string& info)
{
	std::stringstream	ss(info);
	int					max_body_size;
	
	ss >> max_body_size;
	if (ss.fail() || !ss.eof() || max_body_size < 1)
		throw std::runtime_error("Invalid Max Body Size Parsed");
	else
		m_servers.back().addInfo(MAX_SIZE, info);
}

static bool isDirectory(const std::string& path)
{
    struct stat file_stat;

    return (stat(path.c_str(), &file_stat) == 0) && S_ISDIR(file_stat.st_mode);
}

void	Config::checkRoot(std::string& info)
{
	if (info[0] == '/')
		throw std::runtime_error("Action not permitted");

	for (size_t i = 0; i < info.size(); i++)
	{
		i = info.find_first_of('.', i + 1);
		if (i == std::string::npos)
			break ;
		else if (info[i + 1] == '.')
			throw std::runtime_error("Action not permitted");
	}

	if (!isDirectory(info))
		throw std::runtime_error("Root Information Invalid");
}

void	Config::parseErrorPage(std::string& number_str, std::string& path)
{
	std::string	full_path;
	int			error_number = std::atoi(number_str.c_str());

	// std::cout << YELLOW << number_str << "  " << path << RESET << std::endl;

	if (number_str.size() != 3 || error_number < 400 || error_number > 599)
		throw std::runtime_error("Invalid Error Page Number");

	full_path = m_servers.back().getInfo(ROOT);
	if (full_path == NOT_FOUND)
		full_path.clear();
	full_path += path;

	if (access(full_path.c_str(), F_OK | R_OK))
		throw std::runtime_error("Invalid Error Page Path");
	
	m_servers.back().addErrorPage(error_number, full_path);
}
