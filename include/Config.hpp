#ifndef CONFIG_HPP
	#define CONFIG_HPP

	#include <iostream>
	#include <string>
	#include <vector>
	#include <map>

	struct t_Location
	{
		std::vector<std::string>	
	}

	struct t_ServerInfo
	{
		std::string					serverName;
		std::string					ipAddress;
		std::string					port;
		std::vector<std::string>	errorPages;
		unsigned int				bodySize;

		std::vector<t_Location>		locations;
	}

	class Config
	{
		private:
			std::vector<t_ServerInfo> m_servers;
			std::vector<std::string> m_port;
			std::string m_serverName;

		public:
			Config();
			Config(const std::string& configFile);
			~Config();
	};
#endif
