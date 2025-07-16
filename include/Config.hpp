#ifndef CONFIG_HPP
	#define CONFIG_HPP

	#include <iostream>
	#include <fstream>
	#include <string>
	#include <vector>
	#include <cstdlib>
	#include <sys/stat.h>
	#include <map>
	#include <sstream>
	#include <unistd.h>
	#include "Server.hpp"
	#include "define.hpp"

	enum	ParsingState
	{
		GLOBAL,
		SERVER_BLOCK,
		LOCATION_BLOCK,
		LOOKING_FOR_SERVER_BLOCK,
		LOOKING_FOR_LOCATION_BLOCK,
		ERROR
	};

	class Config
	{
		private:
			ParsingState					m_state;
			std::vector<Server>				m_servers;

		public:
			Config();
			Config(const std::string& configFile, Server& servers);
			~Config();
			void	parsingServerInfo(std::stringstream& sequenced_line, ParsingState& state, Server& server);
			void	parsingLocationInfo(std::stringstream& sequenced_line, ParsingState& state, Server& server);
			void	parsingIPAddress(std::string& address);
			void	parsingPort(std::string& port);
			void	parsingServerName(std::string& info);
			void	parsingMaxBodySize(std::string& info);
			void	parseDefaultFile(std::string& info);
			void	checkRoot(std::string& info);
			void	parseErrorPage(std::string& number, std::string& path);
    		// void	ParsingLocationInfo(std::stringstream& content, ParsingState& state, Server& server);
	};

	bool	lookingFor(std::stringstream& content, std::string target);
	void	setupContent(std::fstream& configIn, std::stringstream& content);

#endif
