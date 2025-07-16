#include "Config.hpp"

void	Config::parseDefaultFile(std::string& file)
{
	// get root info, et join root + index puis check permissions
	std::string	root;
	root = m_servers.back().getInfo(ROOT);

}

void	Config::parsingPort(std::string& port_str)
{
	std::stringstream	ss(port_str);
	int					port;
	
	ss >> port;
	if (ss.fail() || !ss.eof() || port < 1 || port > 65535)
		throw std::runtime_error("Invalid Port Parsed");
	else
		m_servers.back().addInfo(PORT, port_str);
}

void	Config::parsingIPAddress(std::string& address)
{
	int			index = 0;
	int			checker;
	std::string	format = "XXX.XXX.XXX.XXX";
	std::string buffer;

	if (address == "localhost")
	{
		m_servers.back().addInfo(HOST, "127.0.0.1");
		return ;
	}

	for (int i = 0; ((isdigit(address[i]) && format[i] == 'X') || format[i] == address[i]); i++)
	{
		if (format[i] == '.' || !format[i])
		{
			checker = std::atoi(buffer.c_str());
			if (1 > checker || checker > 255)
				throw std::runtime_error("Invalid IP Address");
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

	for (unsigned i = 0; i < info.size(); i++)
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

	if (number_str.size() != 3 || (error_number >= 400 && error_number <= 599))
		throw std::runtime_error("Invalid Error Page Number");

	full_path = m_servers.back().getInfo(ROOT);
	if (full_path == NOT_FOUND)
		full_path = path;
	else
		full_path += path;

	if (access(full_path.c_str(), F_OK | R_OK))
		throw std::runtime_error("Invalid Error Page Path");
	
	m_servers.back().addErrorPage(error_number, full_path);
}
