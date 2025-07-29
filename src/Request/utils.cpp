#include "Request.hpp"

void	to_lower(std::string & input) {
	for (std::string::size_type i = 0; i < input.length(); ++i) {
		input[i] = std::tolower(static_cast<unsigned char>(input[i]));
	}
}

bool	isCGI(Request & request){
	(void)request;
	return false;
}

static	std::string	getMethods(Location &loc)
{
	std::string message = "\r\nAllowed Methods: ";

	if (loc.getInfo(GET) != NOT_FOUND)
		message += "GET ";
	if (loc.getInfo(POST) != NOT_FOUND)
		message += "POST ";
	if (loc.getInfo(DELETE) != NOT_FOUND)
		message += "DELETE";

	return message;
}

std::string Request::checkURI()
{
	Server							server    = _servers[_server_index];
	std::string						uri		  = _URI;
	std::map<std::string, Location> locations = server.getLocations();

	if (_URI.find("..") != std::string::npos)
	{
		std::cerr << "double point" << std::endl;
		return ERROR_400;
	}
	
	while (uri.find("/") != std::string::npos)
	{
		size_t pos = uri.find_last_of("/");
		if (pos == std::string::npos)
			break ;
		else
			uri.erase(pos);
		std::cout << BLUE << uri  << RESET<< std::endl;
    	for (std::map<std::string, Location>::iterator it = locations.begin(); it != locations.end(); it++)
    	{
    		std::string loc_path	= it->first;
    		Location	current_loc	= it->second;
    		
    		std::cout << loc_path << " : " << _URI << std::endl;
    		if (loc_path == uri)
    		{
    			current_loc.getInfo(POST);
    			switch (_method.size())
    			{
    				case 3:
    					if (current_loc.getInfo(GET) != NOT_FOUND)
    						return "OK";
    				case 4:
    					if (current_loc.getInfo(POST) != NOT_FOUND)
    						return "OK";
    				case 6:
    					if (current_loc.getInfo(DELETE) != NOT_FOUND)
    						return "OK";
    				default:
    					return ERROR_405 + getMethods(current_loc);
    			}
    		}
    	}
    }
	return ERROR_404;
}

int Request::getServerIndex(std::string address)
{
	int pos = address.find(":");
	address.erase(pos);
	std::cout << address << std::endl;
	if (address == "localhost")
		address = "127.0.0.1";

	for (unsigned i = 0; i < _servers.size(); i++)
	{
		if (_servers[i].getInfo(HOST) == address)
			return i;
	}
	return -1;
}
