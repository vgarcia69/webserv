#ifndef SERVER_HPP
	#define SERVER_HPP

	#include <vector>
	#include <string>
	#include <map>
	#include <sstream>
	#include "Location.hpp"

	typedef std::map<std::string, std::string>	StringMap;
	typedef std::pair<std::string, std::string> StringPair;

	class Server
	{
		private:
			StringMap					m_info;	
			std::vector<Location>		m_locations;
    		std::vector<std::string>	m_errorPages;
			int							m_maxBodySize;
			// std::string				serverName;
    		// std::string				ipAddress;
    		// std::string				port;
    		// std::string 				root;

		public:
			Server();			
			~Server();
			void	addLocation(std::string& root);
			void	addInfo(std::string keyword, std::string& info);

			void	run();
	};


#endif
